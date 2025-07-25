#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

int main() {
  try {
    const std::string host = "api64.ipify.org";
    const std::string port = "443";
    const std::string target = "/?format=json";

    net::io_context ioc;

    // 1. Configure TLS context
    std::cout << "--> Configuring SSL context...\n";
    ssl::context ctx(ssl::context::tlsv13_client);
    ctx.set_default_verify_paths();         // Use system CA
    ctx.set_verify_mode(ssl::verify_peer);  // Verify server certificate

    // 2. Create SSL stream
    std::cout << "--> Creating SSL stream...\n";
    beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

    // Set SNI (Server Name Indication)
    std::cout << "--> Setting SNI for url: " << host << ":" << port << target
              << "\n";
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
      beast::error_code ec{static_cast<int>(::ERR_get_error()),
                           net::error::get_ssl_category()};
      throw beast::system_error{ec};
    }

    // 3. Resolve domain name and connect
    std::cout << "--> Resolving host: " << host << ":" << port << "\n";
    tcp::resolver resolver(ioc);
    auto const results = resolver.resolve(host, port);
    beast::get_lowest_layer(stream).connect(results);

    // 4. SSL handshake
    std::cout << "--> Performing SSL handshake...\n";
    stream.handshake(ssl::stream_base::client);

    // 5. Construct HTTP GET request
    std::cout << "--> Constructing HTTP GET request...\n";
    http::request<http::string_body> req(http::verb::get, target, 11);
    req.set(http::field::host, host);
    req.set(http::field::user_agent, "Beast HTTP Client");

    // 6. Send request
    std::cout << "--> Sending HTTP request...\n";
    http::write(stream, req);

    // 7. Read response
    std::cout << "--> Reading HTTP response...\n";
    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(stream, buffer, res);

    // 8. Print response
    std::cout << "=> Status:\n"
              << res.result_int() << " " << res.reason() << "\n";
    std::cout << "=> Headers:\n" << res.base() << "\n";
    std::cout << "=> Body:\n"
              << beast::buffers_to_string(res.body().data()) << "\n";

    // 9. Simple string parsing (optional)
    std::cout << "--> Parsing response body for IP address...\n";
    std::string body_str = beast::buffers_to_string(res.body().data());
    size_t ip_pos = body_str.find("\"ip\":\"");
    if (ip_pos != std::string::npos) {
      ip_pos += 6;  // Skip "ip":"
      size_t end_pos = body_str.find("\"", ip_pos);
      if (end_pos != std::string::npos) {
        std::string ip = body_str.substr(ip_pos, end_pos - ip_pos);
        std::cout << "Parsed IP: " << ip << "\n";
      }
    }

    // 10. Close connection
    std::cout << "--> Closing connection...\n";
    beast::error_code ec;

    // Perform SSL shutdown first
    stream.shutdown(ec);

    // Handle common SSL shutdown errors gracefully
    if (ec == net::error::eof || ec == ssl::error::stream_truncated ||
        ec == net::error::connection_reset) {
      // These are expected when the server closes the connection
      ec = {};
    }

    // Then shutdown the underlying TCP socket
    beast::get_lowest_layer(stream).socket().shutdown(
        tcp::socket::shutdown_both, ec);

    // Handle TCP shutdown errors
    if (ec && ec != beast::errc::not_connected) {
      std::cerr << "Warning: TCP shutdown error: " << ec.message() << "\n";
      // Don't throw here as the main operation was successful
    }

  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  std::cout << "--> HTTPS request completed successfully.\n";
  return 0;
}
