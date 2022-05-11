#include <vector>
#include <string>
#include "parseInt.h"
#include "val.h"
#include "parseInt.h"
using namespace std;

// add op to this
Value Value :: operator+(const Value& op) const{
    if(IsErr() || op.IsErr()) return Value();
    if(op.IsString() && IsString()) return Value(GetString() + op.GetString());
    if(op.IsInt() && IsInt()) return Value(op.GetInt() + GetInt());
    if(op.IsBool() || IsBool()) return Value();
    if(op.IsReal() && IsReal()) return Value(op.GetReal() + GetReal());
    if(op.IsReal() && IsInt()) return Value(op.GetReal() + GetInt());
    if(op.IsInt() && IsReal()) return Value(op.GetInt() + GetReal());
    return Value();
}

// subtract op from this
Value Value ::operator-(const Value& op) const{
    if(IsErr() || op.IsErr()) return Value();
    // if(op.IsString() && IsString()) return Value(); //no subtract string
    if(op.IsInt() && IsInt()) return Value(op.GetInt() - GetInt());
    if(op.IsBool() || IsBool()) return Value();
    if(op.IsReal() && IsReal()) return Value(op.GetReal() - GetReal());
    if(op.IsReal() && IsInt()) return Value(op.GetReal() - GetInt());
    if(op.IsInt() && IsReal()) return Value(op.GetInt() - GetReal());
    return Value();
} 

// multiply this by op
Value Value :: operator*(const Value& op) const{
    if(IsErr() || op.IsErr()) return Value();
    if(op.IsString() || IsString())return Value();
    if(op.IsInt() && IsInt()) return Value(op.GetInt() * GetInt());
    if(op.IsReal() && IsReal()) return Value(op.GetReal() * GetReal());
    if(op.IsReal() && IsInt()) return Value(op.GetReal() * GetInt());
    if(op.IsInt() && IsReal()) return Value(op.GetInt() * GetReal());
    return Value();
}

// divide this by op
Value Value :: operator/(const Value& op) const{
    if(IsErr() || op.IsErr()) return Value();
    if(op.IsString() || IsString())return Value();
    if(op.IsInt() && IsInt()) return Value(op.GetInt() / GetInt());
    if(op.IsReal() && IsReal()) return Value(op.GetReal() / GetReal());
    if(op.IsReal() && IsInt()) return Value(op.GetReal() / GetInt());
    if(op.IsInt() && IsReal()) return Value(op.GetInt() / GetReal());
    return Value();
}

Value Value :: operator==(const Value& op) const{
    if(IsErr() || op.IsErr()) return Value();
    if(op.IsString() && IsString())return Value(op.GetString() == GetString());
    if(op.IsInt() && IsInt()) return Value(op.GetInt() == GetInt());
    if(op.IsBool() && IsBool()) return Value(op.GetBool() == GetBool());
    if(op.IsReal() && IsReal())return Value(op.GetReal() == GetReal());
    if(op.IsReal() && IsInt()) return Value(op.GetReal() == GetInt());
    if(op.IsInt() && IsReal()) return Value(op.GetInt() == GetReal());
    return Value();
}

Value Value :: operator>(const Value& op) const{
    if(IsErr() || op.IsErr()) return Value();
    if(op.IsString() || IsString())return Value();
    if(op.IsInt() && IsInt()) return Value(op.GetInt() > GetInt());
    if(op.IsBool() && IsBool()) return Value(op.GetBool() > GetBool());
    if(op.IsReal() && IsReal())return Value(op.GetReal() > GetReal());
    if(op.IsReal() && IsInt()) return Value(op.GetReal() > GetInt());
    if(op.IsInt() && IsReal()) return Value(op.GetInt() > GetReal());
    return Value();
}

Value Value :: operator<(const Value& op) const{
    if(IsErr() || op.IsErr()) return Value();
    if(op.IsString() || IsString())return Value();
    if(op.IsInt() && IsInt()) return Value(op.GetInt() < GetInt());
    if(op.IsBool() && IsBool()) return Value(op.GetBool() < GetBool());
    if(op.IsReal() && IsReal())return Value(op.GetReal() < GetReal());
    if(op.IsReal() && IsInt()) return Value(op.GetReal() < GetInt());
    if(op.IsInt() && IsReal()) return Value(op.GetInt() < GetReal());
    return Value();
}

