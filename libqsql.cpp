#include "libqsql.h"
#include<QSqlDatabase>
#include<QVariant>
#include <memory>
using namespace L_library;
LibQSQL::LibQSQL(const QString& text,const QString& name):
    databaseName(name),databaseType(text)
{

    db=new QSqlDatabase(QSqlDatabase::addDatabase(text,name));
    db->setDatabaseName(name);
    db->open();

    query=new QSqlQuery(*db);
    error=new QSqlError();
    record=new QSqlRecord();

    for(auto &i:db->tables())
    {
        tables.push_back(i.toStdString());
    }
}

LibQSQL::~LibQSQL()
{

    db->close();

    delete query;
    delete db;
    delete error;
    delete record;


}

void LibQSQL::createColumn(const QString& table,const QString& realName,double d)
{
    qDebug()<<"creating a float column";

    QString queryString=QString("ALTER "
                                "TABLE '%1' ADD COLUMN '%2' DOUBLE").
            arg(QString(table)).arg(realName);

    query->exec(queryString);
    if(query->lastError().isValid())
    {
        qDebug()<<"error when creating column:"<<query->lastError();
    }

}
bool LibQSQL::insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                   const QString& column1,
                                          const QString& index,const QString varchar)
{
    QString queryString=QString("update '%1' set %2='%3' WHERE %4= %5 ").arg(table)
            .arg(column).arg(varchar).arg(column1).arg(index);
    query->prepare(queryString);


    if(query->exec())
    {
        return true;
    }
    else
    {
        qDebug()<<"INSERT error in :"<<query->lastQuery()<<" with "<<query->lastError();
        return false;
    }
}
bool LibQSQL::insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                        const QString& column1,
                                          const QString& index,const int number)
{
    QString queryString=QString("update '%1' set %2=%3 WHERE %4= %5 ").arg(table)
            .arg(column).arg(number).arg(column1).arg(index);
    query->prepare(queryString);
    if(query->exec(queryString))
    {
        return true;
    }
    else
    {
        qDebug()<<"INSERT error in :"<<query->lastQuery()<<" with "<<query->lastError();
        return false;
    }
}

bool LibQSQL::insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                        const QString& column1,
                                          const QString& index,const bool flag)
{    QString queryString=QString("update '%1' set %2=%3 WHERE %4= %5 ").arg(table)
            .arg(column).arg(flag).arg(column1).arg(index);
    query->prepare(queryString);

    if(query->exec(queryString))
    {
        return true;
    }
    else
    {
        qDebug()<<"INSERT error in :"<<query->lastQuery()<<" with "<<query->lastError();
        return false;
    }
}

bool LibQSQL::insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                        const QString& column1,
                                          const QString& index,const double ftype)
{
    QString queryString=QString("update '%1' set %2=%3 WHERE %4= %5 ").arg(table)
            .arg(column).arg(ftype).arg(column1).arg(index);
    query->prepare(queryString);
    query->bindValue(":index",index);
    if(query->exec())
    {
        return true;
    }
    else
    {
        qDebug()<<"INSERT error in :"<<query->lastQuery()<<" with "<<query->lastError();
        return false;
    }
}

bool LibQSQL::insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                        const QString& column1,
                                          const QString& index,const QByteArray blob)
{
    QString queryString=QString("update '%1' set %2='%3' WHERE %4= %5 ").arg(table)
            .arg(column).arg(blob).arg(column1).arg(index);
    query->prepare(queryString);
    if(query->exec())
    {
        return true;
    }
    else
    {
        qDebug()<<"INSERT error in :"<<query->lastQuery()<<" with "<<query->lastError();
        return false;
    }
}

bool LibQSQL::insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                        const QString& column1,
                                          const QString& index,const unsigned int uNumber)
{
    QString queryString=QString("update '%1' set %2=%3 WHERE %4= %5 ").arg(table)
            .arg(column).arg(uNumber).arg(column1).arg(index);
    query->prepare(queryString);
    if(query->exec())
    {
        return true;
    }
    else
    {
        qDebug()<<"INSERT error in :"<<query->lastQuery()<<" with "<<query->lastError();
        return false;
    }
}

void LibQSQL::printTables() const
{
    qDebug()<<db->tables();
}

bool LibQSQL::insertRowIntoTable_noString(const QString& table,QList<QString>& columns,QList<QString>& items )
{
    QString queryString=QString("INSERT INTO %1(").arg(table);
    for(auto& i:columns)
    {
        queryString.append(QString("%1, ").arg(i));
    }
    queryString.chop(2);
    queryString.append(") values(");
    for(auto& i:items)
    {
        queryString.append(QString("%1, ").arg(i));
    }
    queryString.chop(2);
    queryString.append(")");

    if(!query->exec(queryString))
    {
        qDebug()<<"error when insert in:"<<query->lastQuery()<<"with "<<query->lastError();
        return false;
    }
    return true;
}

bool LibQSQL::insertRowIntoTable_allString(const QString& table,
                                           QList<QString>& columns,QList<QString>& items )
{
    QString queryString=QString("INSERT INTO %1(").arg(table);
    for(auto& i:columns)
    {
        queryString.append(QString("%1, ").arg(i));
    }
    queryString.chop(2);
    queryString.append(") values(");
    for(auto& i:items)
    {
        queryString.append(QString("'%1', ").arg(i));
    }
    queryString.chop(2);
    queryString.append(")");

    if(!query->exec(queryString))
    {
        qDebug()<<"error when insert in:"<<query->lastQuery()<<"with "<<query->lastError();
        return false;
    }
    return true;
}

std::shared_ptr<QSqlQuery> LibQSQL::execRawString(const QString& str)
{
    std::shared_ptr<QSqlQuery> st(new QSqlQuery(*db));
    if(!st->exec(str))
    {
        qDebug()<<"erorr when raw string in:"<<query->lastQuery()<<" with "<<query->lastError();
        return nullptr;
    }
    return st;
}

void LibQSQL::printColumns(const QString& table) const
{
      QString queryString=QString("select * from %1").arg(table);
      if(!query->exec(queryString))
      {
          qDebug()<<"error when:"<<query->lastQuery()<<"with "<<query->lastError();
          return;

     }
      *record=query->record();
      QString tmp;
      for(int i=0;i<record->count();++i)
      {
          tmp.append(record->fieldName(i));
          tmp.append("(");
          tmp.append(record->field(i).metaType().name());
          tmp.append(")");
          tmp.append(" ");
      }
      qDebug()<<"columns in "<<table<<" :"<<tmp;
}










