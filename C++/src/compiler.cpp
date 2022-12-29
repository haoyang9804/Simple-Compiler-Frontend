#include <iostream>
#include <unordered_map>
#include <vector>
#include <list>

template <typename Base, typename T> bool isinstanceof(const T *t) {
  return dynamic_cast<const Base *>(t) != nullptr;
}

namespace Expr {
class Expr {
public:
  virtual ~Expr() {}
  virtual std::string expr_name() { return "Expr"; }
};

class Cst : public Expr {
public:
  Cst() {}
  Cst(int val) : val(val) {}
  int val;
  std::string expr_name() { return "Cst"; }
};

class Add : public Expr {
public:
  Add() {}
  Add(Expr *e1, Expr *e2) : e1(e1), e2(e2) {}
  Expr *e1;
  Expr *e2;
  std::string expr_name() { return "Add"; }
};

class Mul : public Expr {
public:
  Mul() {}
  Mul(Expr *e1, Expr *e2) : e1(e1), e2(e2) {}
  Expr *e1;
  Expr *e2;
  std::string expr_name() { return "Mul"; }
};

class Var : public Expr {
public:
  Var() {}
  Var(std::string name) : name(name) {}
  std::string name;
  std::string expr_name() { return "Var"; }
};

class Let : public Expr {
public:
  Let() {}
  Let(std::string name, Expr *e1, Expr *e2) : name(name), e1(e1), e2(e2) {}
  std::string name;
  Expr *e1;
  Expr *e2;
  std::string expr_name() { return "Let"; }
};

typedef std::unordered_map<std::string, int> Env;

int eval(Expr *eptr, Env env) {
  if (isinstanceof<Cst>(eptr)) {
    Cst *cst = static_cast<Cst *>(eptr);
    return cst->val;
  } else if (isinstanceof<Add>(eptr)) {
    Add *add = static_cast<Add *>(eptr);
    return eval(add->e1, env) + eval(add->e2, env);
  } else if (isinstanceof<Mul>(eptr)) {
    Mul *mul = static_cast<Mul *>(eptr);
    return eval(mul->e1, env) * eval(mul->e2, env);
  } else if (isinstanceof<Var>(eptr)) {
    Var *var = static_cast<Var *>(eptr);
    auto pos = env.find(var->name);
    if (pos == env.end()) {
      throw std::logic_error("Cannot find key " + var->name);
    }
    return pos->second;
  } else if (isinstanceof<Let>(eptr)) {
    Let *let = static_cast<Let *>(eptr);
    int e1_val = eval(let->e1, env);
    env.insert(std::make_pair(let->name, e1_val));
    return eval(let->e2, env);
  } else {
    throw std::logic_error("Unsupported expr in Expr::eval: " +
                           eptr->expr_name());
  }
}

std::string to_str(Expr *eptr) {
  std::string str = "";
  if (isinstanceof<Cst>(eptr)) {
    Cst *cst = static_cast<Cst *>(eptr);
    str += std::to_string(cst->val);
  } else if (isinstanceof<Add>(eptr)) {
    Add *add = static_cast<Add *>(eptr);
    str += to_str(add->e1) + " + " + to_str(add->e2);
  } else if (isinstanceof<Mul>(eptr)) {
    Mul *mul = static_cast<Mul *>(eptr);
    str += to_str(mul->e1) + " * " + to_str(mul->e2);
  } else if (isinstanceof<Var>(eptr)) {
    Var *var = static_cast<Var *>(eptr);
    str += var->name;
  } else if (isinstanceof<Let>(eptr)) {
    Let *let = static_cast<Let *>(eptr);
    str +=
        "let " + let->name + " = " + to_str(let->e1) + " in " + to_str(let->e2);
  } else {
    throw std::logic_error("Unsupported expr in Expr::to_str: " +
                           eptr->expr_name());
  }
  return str;
}

} // namespace Expr

namespace Nameless {
class Expr {
public:
  virtual ~Expr() {}
  virtual std::string expr_name() { return "Expr"; }
};

class Cst : public Expr {
public:
  Cst() {}
  Cst(int val) : val(val) {}
  int val;
  std::string expr_name() { return "Cst"; }
};

class Add : public Expr {
public:
  Add() {}
  Add(Expr *e1, Expr *e2) : e1(e1), e2(e2) {}
  Expr *e1;
  Expr *e2;
  std::string expr_name() { return "Add"; }
};

class Mul : public Expr {
public:
  Mul() {}
  Mul(Expr *e1, Expr *e2) : e1(e1), e2(e2) {}
  Expr *e1;
  Expr *e2;
  std::string expr_name() { return "Mul"; }
};

class Var : public Expr {
public:
  Var() {}
  Var(int index) : index(index) {}
  int index;
  std::string expr_name() { return "Var"; }
};

class Let : public Expr {
public:
  Let() {}
  Let(Expr *e1, Expr *e2) : e1(e1), e2(e2) {}
  Expr *e1;
  Expr *e2;
  std::string expr_name() { return "Let"; }
};

typedef std::vector<int> Env;

int eval(Expr *eptr, Env env) {
  if (isinstanceof<Cst>(eptr)) {
    Cst *cst = static_cast<Cst *>(eptr);
    return cst->val;
  } else if (isinstanceof<Add>(eptr)) {
    Add *add = static_cast<Add *>(eptr);
    return eval(add->e1, env) + eval(add->e2, env);
  } else if (isinstanceof<Mul>(eptr)) {
    Mul *mul = static_cast<Mul *>(eptr);
    return eval(mul->e1, env) * eval(mul->e2, env);
  } else if (isinstanceof<Var>(eptr)) {
    Var *var = static_cast<Var *>(eptr);
    if (env.size() <= var->index) {
      throw std::logic_error("var's index is out of env's scope");
    }
    return env[var->index];
  } else if (isinstanceof<Let>(eptr)) {
    Let *let = static_cast<Let *>(eptr);
    int e1_val = eval(let->e1, env);
    env.push_back(e1_val);
    return eval(let->e2, env);
  } else {
    throw std::logic_error("Unsupported expr in Nameless::eval: " +
                           eptr->expr_name());
  }
}

std::string to_str(Expr *eptr) {
  std::string str = "";
  if (isinstanceof<Cst>(eptr)) {
    Cst *cst = static_cast<Cst *>(eptr);
    str += std::to_string(cst->val);
  } else if (isinstanceof<Add>(eptr)) {
    Add *add = static_cast<Add *>(eptr);
    str += to_str(add->e1) + " + " + to_str(add->e2);
  } else if (isinstanceof<Mul>(eptr)) {
    Mul *mul = static_cast<Mul *>(eptr);
    str += to_str(mul->e1) + " * " + to_str(mul->e2);
  } else if (isinstanceof<Var>(eptr)) {
    Var *var = static_cast<Var *>(eptr);
    str += "Var(" + std::to_string(var->index) + ")";
  } else if (isinstanceof<Let>(eptr)) {
    Let *let = static_cast<Let *>(eptr);
    str += "let " + to_str(let->e1) + " in " + to_str(let->e2);
  } else {
    throw std::logic_error("Unsupported expr in Nameless::to_str: " +
                           eptr->expr_name());
  }
  return str;
}

} // namespace Nameless

namespace Instruction {
class Instr {
public:
  virtual ~Instr() {}
  virtual std::string expr_name() { return "Instr"; }
};

class Cst : public Instr {
public:
  int val;
  Cst() {}
  Cst(int val): val(val) {}
  std::string expr_name() { return "Cst"; }
};

class Add : public Instr {
public:
  std::string expr_name() { return "Add"; }
};

class Mul : public Instr {
public:
  std::string expr_name() { return "Mul"; }
};

class Var : public Instr {
public:
  Var() {}
  Var(int index) : index(index) {}
  std::string expr_name() { return "Var"; }
  int index;
};

class Pop : public Instr {
public:
  std::string expr_name() { return "Pop"; }
};

class Swap : public Instr {
public:
  std::string expr_name() { return "Swap"; }
};

typedef std::list<int> Stack;
typedef std::list<Instr*> InstrPtrs;

int eval(InstrPtrs instrs, Stack stack) {
  for (Instr* instrPtr : instrs) {
    if (isinstanceof<Cst>(instrPtr)) {
      Instruction::Cst* cst = static_cast<Instruction::Cst*>(instrPtr);
      stack.insert(stack.begin(), cst->val);
    }
    else if(isinstanceof<Add>(instrPtr)) {
      if(stack.size() < 2) {
        throw std::logic_error("Inadequate values in stack for Add instruction");
      }
      int val1 = stack.front();
      stack.pop_front();
      int val2 = stack.front();
      stack.pop_front();
      stack.insert(stack.begin(), val1 + val2);
    }
    else if (isinstanceof<Mul>(instrPtr)) {
      if(stack.size() < 2) {
        throw std::logic_error("Inadequate values in stack for Mul instruction");
      }
      int val1 = stack.front();
      stack.pop_front();
      int val2 = stack.front();
      stack.pop_front();
      stack.insert(stack.begin(), val1 * val2);
    }
    else if (isinstanceof<Var>(instrPtr)) {
      Var* varptr = static_cast<Var*> (instrPtr);
      std::list<int>::iterator listi;
      int count = 0;
      for(listi = stack.begin(), count = 0; listi != stack.end(); ++listi, ++count) {
        if (count == varptr -> index) {
          stack.insert(stack.begin(), (*listi));
          break;
        }
      }
    }
    else if (isinstanceof<Pop>(instrPtr)) {
      stack.pop_front();
    }
    else if (isinstanceof<Swap>(instrPtr)) {
      int val1 = stack.front();
      stack.pop_front();
      int val2 = stack.front();
      stack.pop_front();
      stack.insert(stack.begin(), val1);
      stack.insert(stack.begin(), val2);
    }
    else {
      throw std::logic_error("Unsupported expr in Instr::to_str: " +
                           instrPtr->expr_name());
    }
  }
  if (stack.size() != 1) {
    throw std::logic_error("Incorrect number of elements in stack, and size equals " + std::to_string(stack.size()));
  }
  return *(stack.begin());
}

std::string to_str(InstrPtrs instrs) {
  std::string str = "";
  for (Instr* instr : instrs) {
    if (isinstanceof<Cst>(instr)) {
      Cst* cstptr = static_cast<Cst*>(instr);
      str += ">| Cst " + std::to_string(cstptr->val) + "\n";
    }
    else if (isinstanceof<Add>(instr)) {
      str += ">| Add\n";
    }
    else if (isinstanceof<Mul>(instr)) {
      str += ">| Mul\n";
    }
    else if (isinstanceof<Var>(instr)) {
      Var* varptr = static_cast<Var*> (instr);
      str += ">| Var" + std::to_string(varptr->index) + "\n";
    }
    else if (isinstanceof<Pop>(instr)) {
      str += ">| Pop\n";
    }
    else if (isinstanceof<Swap>(instr)) {
      str += ">| Swap\n";
    }
    else {
      throw std::logic_error("Unsupported instr in Instr::to_str: " + instr->expr_name());
    }
  }
  return str;
}

}

namespace Compiler {

typedef std::vector<std::string> CEnv;

int findIndex(CEnv cenv, std::string name) {
  int index = 0;
  for (std::string str : cenv) {
    if (str == name) {
      return index;
    }
    index++;
  }
  throw std::logic_error("Cannot find name " + name + " in cenv");
}

Nameless::Expr *lowerFromExprToNameless(Expr::Expr *eptr, CEnv cenv) {
  if (isinstanceof<Expr::Cst>(eptr)) {
    Expr::Cst *cst = static_cast<Expr::Cst *>(eptr);
    return new Nameless::Cst(cst->val);
  } else if (isinstanceof<Expr::Add>(eptr)) {
    Expr::Add *add = static_cast<Expr::Add *>(eptr);
    return new Nameless::Add(lowerFromExprToNameless(add->e1, cenv),
                             lowerFromExprToNameless(add->e2, cenv));
  } else if (isinstanceof<Expr::Mul>(eptr)) {
    Expr::Mul *mul = static_cast<Expr::Mul *>(eptr);
    return new Nameless::Mul(lowerFromExprToNameless(mul->e1, cenv),
                             lowerFromExprToNameless(mul->e2, cenv));
  } else if (isinstanceof<Expr::Var>(eptr)) {
    Expr::Var *var = static_cast<Expr::Var *>(eptr);
    return new Nameless::Var(findIndex(cenv, var->name));
  } else if (isinstanceof<Expr::Let>(eptr)) {
    Expr::Let *let = static_cast<Expr::Let *>(eptr);
    cenv.push_back(let->name);
    return new Nameless::Let(lowerFromExprToNameless(let->e1, cenv),
                             lowerFromExprToNameless(let->e2, cenv));
  } else {
    throw std::logic_error("Unsupported expr in Nameless::eval: " +
                           eptr->expr_name());
  }
}

class AbstractVal {
public:
  virtual ~AbstractVal() {}
};

class Slocal : public AbstractVal {

};

class Stmp : public AbstractVal {

};

typedef std::list<AbstractVal*> AEnv;

int findIndexofInstructionVar(int NamelessVarIndex, AEnv aenv) {
  int index = -1;
  size_t aenv_siz = aenv.size();
  int count;
  auto li = aenv.rbegin();
  for (count = 0; li != aenv.rend(); ++li, ++count) {
    if (isinstanceof<Slocal>(*(li))) {
      index ++;
    }
    else if (isinstanceof<Stmp>(*(li))) {
      continue;
    }
    else {
      throw std::logic_error("Unsupported AbstractVar in findIndexofInstructionVar");
    }
    if (index == NamelessVarIndex) {
      return aenv_siz - 1 - count;
    }
  }
  throw std::logic_error("Cannot find NamelessVarIndex " + std::to_string(NamelessVarIndex) + " in aenv");
}

Instruction::InstrPtrs lowerFromNamelessToInstruction(Nameless::Expr* eptr, AEnv aenv) {
  if (isinstanceof<Nameless::Cst>(eptr)) {
    Nameless::Cst* cst = static_cast<Nameless::Cst*>(eptr);
    return {new Instruction::Cst(cst->val)};
  }
  else if (isinstanceof<Nameless::Add>(eptr)) {
    Nameless::Add* add = static_cast<Nameless::Add*> (eptr);
    auto e1 = add->e1;
    auto e2 = add->e2;
    Instruction::InstrPtrs instrPtrs = lowerFromNamelessToInstruction(e1, aenv);
    aenv.push_front(new Stmp());
    instrPtrs.splice(instrPtrs.end(), lowerFromNamelessToInstruction(e2, aenv));
    instrPtrs.push_back(new Instruction::Add());
    return instrPtrs;
  }
  else if (isinstanceof<Nameless::Mul>(eptr)) {
    Nameless::Mul* mul = static_cast<Nameless::Mul*> (eptr);
    auto e1 = mul->e1;
    auto e2 = mul->e2;
    Instruction::InstrPtrs instrPtrs = lowerFromNamelessToInstruction(e1, aenv);
    auto it = instrPtrs.end();
    aenv.push_front(new Stmp());
    instrPtrs.splice(it, lowerFromNamelessToInstruction(e2, aenv));
    instrPtrs.push_back(new Instruction::Mul());
    return instrPtrs;
  }
  else if(isinstanceof<Nameless::Var>(eptr)) {
    Nameless::Var* var = static_cast<Nameless::Var*>(eptr);
    int index = findIndexofInstructionVar(var->index, aenv);
    return {new Instruction::Var(index)};
  }
  else if (isinstanceof<Nameless::Let>(eptr)) {
    Nameless::Let* let = static_cast<Nameless::Let*>(eptr);
    auto e1 = let -> e1;
    auto e2 = let -> e2;
    Instruction::InstrPtrs instrPtrs = lowerFromNamelessToInstruction(e1, aenv);
    aenv.push_front(new Slocal());
    Instruction::InstrPtrs instrPtrs2 = lowerFromNamelessToInstruction(e2, aenv);
    instrPtrs.splice(instrPtrs.end(), instrPtrs2);
    instrPtrs.push_back(new Instruction::Swap());
    instrPtrs.push_back(new Instruction::Pop());
    return instrPtrs;
  }
  throw std::logic_error("Unsupported Nameless::Expr in lowerFromNamelessToInstruction: " + eptr->expr_name());
}

} // namespace Compiler