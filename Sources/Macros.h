#pragma once

#define DEFINE_MEMBER(type, name) \
protected: \
    type m##name; \
\
public: \
    inline const type &Get##name(void) const { return m##name; } \
\
public: \
    inline type &Get##name##_NonConst(void) { return m##name; } \
\
public: \
    inline void Set##name(const type &var) { m##name = var; }


#define DEFINE_MEMBER_CONST(type, name) \
protected: \
    type m##name; \
\
public: \
    inline const type &Get##name(void) const { return m##name; }