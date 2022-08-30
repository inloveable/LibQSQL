#ifndef L_SQLTE_H
#define L_SQLTE_H

#include "libqsql.h"

class L_SQLTE : public L_library::LibQSQL
{
public:
    L_SQLTE(const QString& text,const QString& name);
};

#endif // L_SQLTE_H
