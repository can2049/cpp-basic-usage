#include <iostream>
#include <memory>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>

// 支持的数据类型
using Value = std::variant<int, double, std::string>;
enum class DataType { INT, DOUBLE, STRING };

// 列定义结构体
struct ColumnDef {
  std::string name;
  DataType type;
};

// 表结构体
class Table {
 public:
  std::vector<ColumnDef> columns;
  std::vector<std::vector<Value>> rows;
};

// 数据库结构体
class Database {
 public:
  std::unordered_map<std::string, std::unique_ptr<Table>> tables;
};

class SQLParser {
 public:
  struct CreateTableStmt {
    std::string name;
    std::vector<ColumnDef> columns;
  };

  struct InsertStmt {
    std::string table;
    std::vector<Value> values;
  };

  struct SelectStmt {
    std::string table;
    std::vector<std::string> columns;
    std::string whereColumn;
    std::string whereOp;
    Value whereValue;
  };

  CreateTableStmt parseCreate(const std::string& sql) {
    std::regex re(R"(CREATE TABLE (\w+) \((.*)\))");
    std::smatch match;
    if (!std::regex_match(sql, match, re))
      throw std::runtime_error("Invalid CREATE TABLE syntax");

    CreateTableStmt stmt;
    stmt.name = match[1];

    std::string columnsStr = match[2];
    std::regex colRe(R"((\w+)\s+(INT|DOUBLE|STRING))");
    std::sregex_iterator it(columnsStr.begin(), columnsStr.end(), colRe);
    std::sregex_iterator it_end;

    for (; it != it_end; ++it) {
      ColumnDef col;
      col.name = (*it)[1];
      col.type = (*it)[2] == "INT"      ? DataType::INT
                 : (*it)[2] == "DOUBLE" ? DataType::DOUBLE
                                        : DataType::STRING;
      stmt.columns.push_back(std::move(col));
    }
    return stmt;
  }

  InsertStmt parseInsert(const std::string& sql) {
    std::regex re(R"(INSERT INTO (\w+)\s*VALUES\s*\((.+)\))",
                  std::regex::icase);
    std::smatch match;
    if (!std::regex_match(sql, match, re)) {
      throw std::runtime_error("Invalid INSERT syntax");
    }

    InsertStmt stmt;
    stmt.table = match[1];

    std::string valuesStr = match[2];
    std::regex valueRe(R"(\s*('(?:[^']|'')*'|[\w\.+-]+)\s*,?)");

    std::sregex_iterator it(valuesStr.begin(), valuesStr.end(), valueRe);
    std::sregex_iterator it_end;
    for (; it != it_end; ++it) {
      std::string val = trim((*it)[1]);
      stmt.values.push_back(parseValue(val));
    }
    return stmt;
  }

  SelectStmt parseSelect(const std::string& sql) {
    std::regex re(
        R"(SELECT\s+(.+?)\s+FROM\s+(\w+)(?:\s+WHERE\s+(\w+)\s*([!=<>]+)\s*('(?:[^']|'')*'|[\w\.+-]+))?)",
        std::regex::icase);
    std::smatch match;
    if (!std::regex_match(sql, match, re)) {
      throw std::runtime_error("Invalid SELECT syntax");
    }

    SelectStmt stmt;
    // 解析列
    std::string cols = trim(match[1]);
    // 如果使用 "*"，直接保存该标记
    if (cols == "*") {
      stmt.columns.push_back("*");
    } else {
      std::regex colRe(R"(\s*(\w+)\s*,?)");
      std::sregex_iterator it(cols.begin(), cols.end(), colRe);
      std::sregex_iterator it_end;
      for (; it != it_end; ++it) {
        stmt.columns.push_back((*it)[1]);
      }
    }

    // 基础表名
    stmt.table = match[2];

    // 处理 WHERE 子句（可选）
    if (match[3].matched) {
      stmt.whereColumn = match[3];
      stmt.whereOp = match[4];
      stmt.whereValue = parseValue(trim(match[5]));
    }

    return stmt;
  }

 private:
  // 辅助函数：解析字符串为 Value 类型
  Value parseValue(const std::string& s) {
    if (s.front() == '\'' && s.back() == '\'') {
      return s.substr(1, s.length() - 2);  // 去除引号
    }
    if (s.find('.') != std::string::npos) {
      try {
        return std::stod(s);
      } catch (...) {
      }
    }
    try {
      return std::stoi(s);
    } catch (...) {
    }
    return s;  // 作为字符串返回
  }

  // 辅助函数：去除首尾空格和引号
  static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t'");
    size_t end = s.find_last_not_of(" \t'");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
  }
};

class Executor {
 public:
  void executeCreate(Database& db, const SQLParser::CreateTableStmt& stmt) {
    auto table = std::make_unique<Table>();
    table->columns = stmt.columns;
    db.tables[stmt.name] = std::move(table);
  }

  void executeInsert(Database& db, const SQLParser::InsertStmt& stmt) {
    auto& table = db.tables[stmt.table];
    if (!table) throw std::runtime_error("Table not exists");

    if (stmt.values.size() != table->columns.size())
      throw std::runtime_error("Column count mismatch");

    table->rows.push_back(stmt.values);
  }

  void executeSelect(Database& db, const SQLParser::SelectStmt& stmt) {
    auto& table = db.tables[stmt.table];
    if (!table) throw std::runtime_error("Table not exists");

    // 判断如果是 SELECT * 则使用表中所有列
    std::vector<std::string> columnsToPrint;
    if (stmt.columns.size() == 1 && stmt.columns[0] == "*") {
      for (const auto& colDef : table->columns) {
        columnsToPrint.push_back(colDef.name);
      }
    } else {
      columnsToPrint = stmt.columns;
    }

    // 打印表头
    for (const auto& col : columnsToPrint) {
      std::cout << col << "\t";
    }
    std::cout << "\n------------------------\n";

    // 过滤和输出结果
    for (auto& row : table->rows) {
      if (matchWhereCondition(db, row, stmt)) {
        for (const auto& col : columnsToPrint) {
          printValue(getColumnValue(col, row, table->columns));
        }
        std::cout << "\n";
      }
    }
  }

 private:
  bool matchWhereCondition(Database& db, const std::vector<Value>& row,
                           const SQLParser::SelectStmt& stmt) {
    // 没有WHERE条件时返回全部结果
    if (stmt.whereColumn.empty()) return true;

    // 获取列值（可能抛出column not found异常）
    Value colVal =
        getColumnValue(stmt.whereColumn, row, db.tables[stmt.table]->columns);
    Value condVal = stmt.whereValue;

    // 定义通用比较器（支持数值类型和字符串的跨类型比较）
    auto comparator = [&](auto&& op) -> bool {
      return std::visit(
          [&op](auto&& a, auto&& b) -> bool {
            using AType = std::decay_t<decltype(a)>;
            using BType = std::decay_t<decltype(b)>;

            // 处理数值类型比较（包括int/double混合比较）
            if constexpr (std::is_arithmetic_v<AType> &&
                          std::is_arithmetic_v<BType>) {
              return op(static_cast<double>(a), static_cast<double>(b));
            }
            // 处理字符串比较
            else if constexpr (std::is_same_v<AType, std::string> &&
                               std::is_same_v<BType, std::string>) {
              return op(a, b);
            }
            // 其他类型组合视为不匹配
            else {
              return false;
            }
          },
          colVal, condVal);
    };

    // 分派不同操作符
    if (stmt.whereOp == "=") {
      return comparator([](auto a, auto b) { return a == b; });
    } else if (stmt.whereOp == "!=") {
      return comparator([](auto a, auto b) { return a != b; });
    } else if (stmt.whereOp == ">") {
      return comparator([](auto a, auto b) { return a > b; });
    } else if (stmt.whereOp == "<") {
      return comparator([](auto a, auto b) { return a < b; });
    } else if (stmt.whereOp == ">=") {
      return comparator([](auto a, auto b) { return a >= b; });
    } else if (stmt.whereOp == "<=") {
      return comparator([](auto a, auto b) { return a <= b; });
    } else {
      throw std::runtime_error("Unsupported operator: " + stmt.whereOp);
    }
  }

  void printValue(const Value& val) {
    std::visit([](auto&& arg) { std::cout << arg << "\t"; }, val);
  }

  Value getColumnValue(const std::string& colName,
                       const std::vector<Value>& row,
                       const std::vector<ColumnDef>& cols) {
    for (size_t i = 0; i < cols.size(); ++i) {
      if (cols[i].name == colName) return row[i];
    }
    throw std::runtime_error("Column not found");
  }
};

int main() {
  Database db;
  SQLParser parser;
  Executor executor;

  // 创建表
  executor.executeCreate(
      db, parser.parseCreate(
              "CREATE TABLE users (id INT, name STRING, age INT, sex STRING)"));

  // 插入数据
  auto run_insert_cmd = [&](const std::string& insert_cmd) {
    std::cout << "Insert: " << insert_cmd << std::endl;
    executor.executeInsert(db, parser.parseInsert(insert_cmd));
  };

  run_insert_cmd("INSERT INTO users VALUES (1, 'Alice', 25, 'F')");
  run_insert_cmd("INSERT INTO users VALUES (2, 'Bob', 20, 'M')");
  run_insert_cmd("INSERT INTO users VALUES (3, 'Cai', 19, 'F')");
  run_insert_cmd("INSERT INTO users VALUES (4, 'David', 18, 'M')");

  // 查询数据
  auto run_select_cmd = [&](const std::string& query_cmd) {
    std::cout << "Query: " << query_cmd << std::endl;
    executor.executeSelect(db, parser.parseSelect(query_cmd));
  };

  run_select_cmd("SELECT * FROM users");
  run_select_cmd("SELECT * FROM users WHERE sex = 'F'");
  run_select_cmd("SELECT id, name, sex FROM users WHERE age >= 20");
  run_select_cmd("SELECT id, name, age FROM users WHERE sex = 'M'");
  return 0;
}
