#include <boost/property_map/property_map.hpp>
#include <iostream>
#include <map>
#include <string>

template <typename AddressMap>
void foo(AddressMap address) {
  typedef typename boost::property_traits<AddressMap>::value_type value_type;
  typedef typename boost::property_traits<AddressMap>::key_type key_type;

  value_type old_address, new_address;

  key_type fred = "Fred";
  old_address = get(address, fred);
  new_address = "384 Fitzpatrick Street";
  put(address, fred, new_address);

  key_type joe = "Joe";
  value_type& joes_address = address[joe];
  joes_address = "325 Cushing Avenue";
}

int main() {
  std::map<std::string, std::string> name2address;

  boost::associative_property_map<std::map<std::string, std::string>>
      address_map(name2address);

  name2address.insert({"Fred", "710 West 13th Street"});
  name2address.insert({"Joe", "710 West 13th Street"});

  foo(address_map);

  for (const auto& [name, address] : name2address)
    std::cout << name << ": " << address << "\n";

  return EXIT_SUCCESS;
}
