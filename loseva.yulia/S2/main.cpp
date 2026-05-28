#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <limits>
#include <stdexcept>
#include "list.hpp"

enum TokenType {
  TOK_NUMBER,
  TOK_OPERATOR
};

struct PostfixToken {
  TokenType type;
  long long value;
  char op;
};

long long safe_add(long long a, long long b) {
  if ((b > 0 && a > std::numeric_limits<long long>::max() - b) ||
      (b < 0 && a < std::numeric_limits<long long>::min() - b)) {
    throw std::overflow_error("overflow");
  }
  return a + b;
}

long long safe_sub(long long a, long long b) {
  if ((b < 0 && a > std::numeric_limits<long long>::max() + b) ||
      (b > 0 && a < std::numeric_limits<long long>::min() + b)) {
    throw std::overflow_error("overflow");
  }
  return a - b;
}

long long safe_mul(long long a, long long b) {
  if (a != 0 && b != 0) {
    if (a > 0 && b > 0 && a > std::numeric_limits<long long>::max() / b) throw std::overflow_error("overflow");
    if (a > 0 && b < 0 && b < std::numeric_limits<long long>::min() / a) throw std::overflow_error("overflow");
    if (a < 0 && b > 0 && a < std::numeric_limits<long long>::min() / b) throw std::overflow_error("overflow");
    if (a < 0 && b < 0) {
      if (a == std::numeric_limits<long long>::min() || b == std::numeric_limits<long long>::min()) throw std::overflow_error("overflow");
      if (-a > std::numeric_limits<long long>::max() / (-b)) throw std::overflow_error("overflow");
    }
  }
  return a * b;
}

long long safe_div(long long a, long long b) {
  if (b == 0) {
    throw std::runtime_error("division by zero");
  }
  if (a == std::numeric_limits<long long>::min() && b == -1) {
    throw std::overflow_error("overflow");
  }
  return a / b;
}

long long safe_mod(long long a, long long b) {
  if (b == 0) {
    throw std::runtime_error("modulo by zero");
  }
  if (a == std::numeric_limits<long long>::min() && b == -1) {
    return 0;
  }
  long long res = a % b;
  if (res < 0) {
    res += (b > 0 ? b : -b);
  }
  return res;
}

bool is_number(const std::string& s, long long& val) {
  if (s.empty()) return false;
  try {
    size_t idx = 0;
    val = std::stoll(s, &idx);
    return idx == s.size();
  } catch (...) {
    return false;
  }
}

int main(int argc, char* argv[]) {
  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Error: Cannot open file\n";
      return 1;
    }
    input = &file;
  } else if (argc > 2) {
    std::cerr << "Error: Too many arguments\n";
    return 1;
  }

  loseva::List<long long> results;
  std::string line;

  while (std::getline(*input, line)) {
    bool only_whitespace = true;
    for (char c : line) {
      if (!std::isspace(static_cast<unsigned char>(c))) {
        only_whitespace = false;
        break;
      }
    }
    if (only_whitespace) {
      continue;
    }

    std::stringstream ss(line);
    std::string token_str;
    
    loseva::List<char> op_stack;
    loseva::List<PostfixToken> postfix;
    
    bool expect_operand = true;
    bool valid = true;

    while (ss >> token_str) {
      if (token_str == "(") {
        if (!expect_operand) {
          valid = false;
          break;
        }
        op_stack.push_back('(');
      } else if (token_str == ")") {
        if (expect_operand) {
          valid = false;
          break;
        }
        bool found_lparen = false;
        while (!op_stack.empty()) {
          char top_op = op_stack.back();
          op_stack.pop_back();
          if (top_op == '(') {
            found_lparen = true;
            break;
          }
          postfix.push_back({TOK_OPERATOR, 0, top_op});
        }
        if (!found_lparen) {
          valid = false;
          break;
        }
      } else if (token_str == "+" || token_str == "-" || token_str == "*" || token_str == "/" || token_str == "%") {
        if (expect_operand) {
          valid = false;
          break;
        }
        char current_op = token_str[0];
        int current_prec = (current_op == '+' || current_op == '-') ? 1 : 2;
        
        while (!op_stack.empty()) {
          char top_op = op_stack.back();
          if (top_op == '(') {
            break;
          }
          int top_prec = (top_op == '+' || top_op == '-') ? 1 : 2;
          if (top_prec >= current_prec) {
            postfix.push_back({TOK_OPERATOR, 0, top_op});
            op_stack.pop_back();
          } else {
            break;
          }
        }
        op_stack.push_back(current_op);
        expect_operand = true;
      } else {
        long long val = 0;
        if (!is_number(token_str, val)) {
          valid = false;
          break;
        }
        if (!expect_operand) {
          valid = false;
          break;
        }
        postfix.push_back({TOK_NUMBER, val, '\0'});
        expect_operand = false;
      }
    }

    if (!valid || expect_operand) {
      std::cerr << "Error: Invalid syntax\n";
      return 1;
    }

    while (!op_stack.empty()) {
      char top_op = op_stack.back();
      op_stack.pop_back();
      if (top_op == '(') {
        std::cerr << "Error: Mismatched parentheses\n";
        return 1;
      }
      postfix.push_back({TOK_OPERATOR, 0, top_op});
    }

    loseva::List<long long> eval_stack;
    for (auto it = postfix.begin(); it != postfix.end(); ++it) {
      PostfixToken tok = *it;
      if (tok.type == TOK_NUMBER) {
        eval_stack.push_back(tok.value);
      } else {
        if (eval_stack.empty()) {
          std::cerr << "Error: Invalid evaluation\n";
          return 1;
        }
        long long b = eval_stack.back();
        eval_stack.pop_back();
        
        if (eval_stack.empty()) {
          std::cerr << "Error: Invalid evaluation\n";
          return 1;
        }
        long long a = eval_stack.back();
        eval_stack.pop_back();
        
        long long res = 0;
        try {
          if (tok.op == '+') res = safe_add(a, b);
          else if (tok.op == '-') res = safe_sub(a, b);
          else if (tok.op == '*') res = safe_mul(a, b);
          else if (tok.op == '/') res = safe_div(a, b);
          else if (tok.op == '%') res = safe_mod(a, b);
        } catch (const std::exception& e) {
          std::cerr << "Error: " << e.what() << "\n";
          return 1;
        }
        eval_stack.push_back(res);
      }
    }

    if (eval_stack.empty()) {
      std::cerr << "Error: Invalid result\n";
      return 1;
    }
    long long final_res = eval_stack.back();
    eval_stack.pop_back();
    if (!eval_stack.empty()) {
      std::cerr << "Error: Invalid final state\n";
      return 1;
    }

    results.push_front(final_res);
  }

  if (!results.empty()) {
    for (auto it = results.begin(); it != results.end(); ++it) {
      if (it != results.begin()) {
        std::cout << " ";
      }
      std::cout << *it;
    }
    std::cout << "\n";
  }

  return 0;
}
