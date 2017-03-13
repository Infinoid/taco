#include "fixed_iterator.h"

#include "taco/util/strings.h"

using namespace taco::ir;

namespace taco {
namespace storage {

FixedIterator::FixedIterator(std::string name, const Expr& tensor, int level,
                             size_t fixedSize, Iterator previous)
    : IteratorImpl(previous, tensor) {
  this->tensor = tensor;
  this->level = level;

  std::string idxVarName = name + util::toString(tensor);
  ptrVar = Var::make(util::toString(tensor) + std::to_string(level+1)+"_ptr",
                     typeOf<int>(), false);
  idxVar = Var::make(idxVarName, typeOf<int>(), false);

  this->fixedSize = (int)fixedSize;
}

bool FixedIterator::isDense() const {
  return false;
}

bool FixedIterator::isRandomAccess() const {
  return false;
}

bool FixedIterator::isSequentialAccess() const {
  return true;
}

Expr FixedIterator::getPtrVar() const {
  return ptrVar;
}

Expr FixedIterator::getIdxVar() const {
  return idxVar;
}

Expr FixedIterator::getIteratorVar() const {
  return ptrVar;
}

Expr FixedIterator::begin() const {
  return 0;
}

Expr FixedIterator::end() const {
  return fixedSize;
}

Stmt FixedIterator::initDerivedVars() const {
  Expr ptrVal = Add::make(Mul::make(getParent().getPtrVar(), end()),
                          getIdxVar());
  return VarAssign::make(getIdxVar(), ptrVal);
}

ir::Stmt FixedIterator::storePtr() const {
  return Stmt();
}

ir::Stmt FixedIterator::storeIdx(ir::Expr idx) const {
  return Store::make(getIdxArr(), getPtrVar(), idx);
}

ir::Expr FixedIterator::getPtrArr() const {
  return GetProperty::make(tensor, TensorProperty::Pointer, level);
}

ir::Expr FixedIterator::getIdxArr() const {
  return GetProperty::make(tensor, TensorProperty::Index, level);
}

ir::Stmt FixedIterator::resizePtrStorage(ir::Expr size) const {
  return Stmt();
}

ir::Stmt FixedIterator::resizeIdxStorage(ir::Expr size) const {
  return Allocate::make(getIdxArr(), size, true);
}

}}
