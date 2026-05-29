#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <fstream>
#include "list.hpp"

long long safe_add(long long a, long long b) {
  if (b > 0 && a > std::numeric_limits<long long>::max() - b) throw std::overflow_error("overflow");
  if (b < 0 && a < std::numeric_limits<long long>::min() - b) throw std::underflow_error("underflow");
  return a + b;
}

long long safe_sub(long long a, long long b) {
  if (b > 0 && a < std::numeric_limits<long long>::min() + b) throw std::underflow_error("underflow");
  if (b < 0 && a > std::numeric_limits<long long>::max() + b) throw std::overflow_error("overflow");
  return a - b;
}

long long safe_mul(long long a, long long b) {
  if (a > 0) {
    if (b > 0) {
      if (a > std::numeric_limits<long long>::max() / b) throw std::overflow_error("overflow");
    } else {
      if (b < std::numeric_limits<long long>::min() / a) throw std::underflow_error("underflow");
    }
  } else {
    if (b > 0) {
      if (a < std::numeric_limits<long long>::min() / b) throw std::underflow_error("underflow");
    } else {
      if (a != 0 && b < std::numeric_limits<long long>::max() / a) throw std::overflow_error("overflow");
    }
  }
  return a * b;
}

long long safe_div(long long a, long long b) {
  if (b == 0) throw std::runtime_error("division by zero");
  if (a == std::numeric_limits<long long>::min() && b == -1) throw std::overflow_error("overflow");
  return a / b;
}

long long safe_mod(long long a, long long b) {
  if (b == 0) throw std::runtime_error("division by zero");
  if (a == std::numeric_limits<long long>::min() && b == -1) return 0;
  return a % b;
}

bool is_operator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

int precedence(char op) {
  if (op == '+' || op == '-') return 1;
  if (op == '*' || op == '/' || op == '%') return 2;
  return 0;
}

long long evaluate_expression(const std::string& line) {
  std::stringstream ss(line);
  std::string token;

  loseva::List<std::string> postfix;
  loseva::List<char> op_stack;

  while (ss >> token) {
    if (token.empty()) continue;

    if (token.length() == 1 && is_operator(token[0])) {
      char op1 = token[0];
      while (!op_stack.empty() && op_stack.back() != '(' && precedence(op_stack.back()) >= precedence(op1)) {
        std::string top_op(1, op_stack.back());
        postfix.push_back(top_op);
        op_stack.pop_back();
      }
      op_stack.push_back(op1);
    } else if (token == "(") {
      op_stack.push_back('(');
    } else if (token == ")") {
      bool found = false;
      while (!op_stack.empty()) {
        if (op_stack.back() == '(') {
          found = true;
          op_stack.pop_back();
          break;
        }
        std::string top_op(1, op_stack.back());
        postfix.push_back(top_op);
        op_stack.pop_back();
      }
      if (!found) {
        throw std::runtime_error("Mismatched parentheses");
      }
    } else {
      size_t idx = 0;
      if (token[0] == '-' || token[0] == '+') {
        if (token.length() < 2) throw std::runtime_error("Invalid integer token");
        idx = 1;
      }
      for (; idx < token.length(); ++idx) {
        if (!std::isdigit(token[idx])) {
          throw std::runtime_error("Invalid token: non-integer");
        }
      }
      postfix.push_back(token);
    }
  }

  while (!op_stack.empty()) {
    if (op_stack.back() == '(' || op_stack.back() == ')') {
      throw std::runtime_error("Mismatched parentheses");
    }
    std::string top_op(1, op_stack.back());
    postfix.push_back(top_op);
    op_stack.pop_back();
  }

  if (postfix.empty()) {
    throw std::runtime_error("Empty expression");
  }

  loseva::List<long long> eval_stack;
  for (auto it = postfix.begin(); it != postfix.end(); ++it) {
    std::string t = *it;
    if (t.length() == 1 && is_operator(t[0])) {
      if (eval_stack.size() < 2) {
        throw std::runtime_error("Invalid expression structure");
      }
      long long b = eval_stack.back();
      eval_stack.pop_back();
      long long a = eval_stack.back();
      eval_stack.pop_back();

      long long res = 0;
      char op = t[0];
      if (op == '+') res = safe_add(a, b);
      else if (op == '-') res = safe_sub(a, b);
      else if (op == '*') res = safe_mul(a, b);
      else if (op == '/') res = safe_div(a, b);
      else if (op == '%') res = safe_mod(a, b);

      eval_stack.push_back(res);
    } else {
      try {
        long long val = std::stoll(t);
        eval_stack.push_back(val);
      } catch (...) {
        throw std::overflow_error("Literal overflow");
      }
    }
  }

  if (eval_stack.size() != 1) {
    throw std::runtime_error("Invalid expression structure");
  }

  return eval_stack.back();
}

int main(int argc, char* argv[]) {
  std::istream* input = &std::cin;
  std::ifstream infile;
  if (argc > 1) {
    infile.open(argv[1]);
    if (!infile.is_open()) {
      std::cerr << "Error opening file\n";
      return 1;
    }
    input = &infile;
  }

  std::string line;
  loseva::List<long long> results;

  try {
    while (std::getline(*input, line)) {
      bool empty_or_whitespace = true;
      for (char c : line) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
          empty_or_whitespace = false;
          break;
        }
      }
      if (empty_or_whitespace) {
        continue;
      }

      long long res = evaluate_expression(line);
      results.push_back(res);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  if (results.empty()) {
    return 0;
  }

  while (!results.empty()) {
    std::cout << results.back();
    results.pop_back();
    if (!results.empty()) {
      std::cout << " ";
    }
  }
  std::cout << "\n";

  return 0;
}
