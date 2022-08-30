#ifndef LIBQSQL_H
#define LIBQSQL_H



#include<map>
#include <memory>
#include<vector>
#include<string>
#include<list>
#include<QString>
#include<type_traits>

#ifndef _begin_LL
#define _begin_LL namespace L_library{
#endif
#ifndef _end_LL
#define _end_LL }
#endif


#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QSqlRecord>
#include<QSqlField>
_begin_LL
class  LibQSQL
{
public:
   LibQSQL(const QString& text,const QString& name);
   ~LibQSQL();


   //note:The number of columns must match perfectly as the size of names.

   //as:QVector<QString> tmp;tmp<<"test1"<<"test2"<<"test3"
   //   int a;QString b;
   //   obj.createTable("test",tmp,a,b,b)

   //note2:The last two paras pay no effect but providing the types of fileds.
   //note3:Explicitly declare a variable when you attempt to pass string literals.
   template<typename field,typename ...fields>
   void initDatabase(const QString& firstTable, QVector<QString> names,field column,fields ...columns);
   template<typename field,typename ...fields>
   void createTable(const QString& tableName, QVector<QString> names,field column,fields ...columns);




   template<typename T>
   void createColumn(const QString& table,const QString& realName,T type);
   void createColumn(const QString& table,const QString& realName,double d);
   //templates do not accpet double

   void printTables() const;
   void printColumns(const QString& table) const;
   //note:the types of the columns are not assured.So it's better for you to check with a
   //database browser in person;

   //these two functions provide access to append a row in convenience,but are apparently over clumsy.
   //I'll try to relpace them with templates in the future,along with those overloads below.
   //But for now (2022/8/30,v0.9),please use functions below;
   bool insertRowIntoTable_allString(const QString& table,QList<QString>& columns,QList<QString>& items );
   bool insertRowIntoTable_noString(const QString& table,QList<QString>& columns,QList<QString>& items );
   //you have to make sure that every field of your data must be the type of varchar
   //or something else smiliar if you use this function,otherwise,use funcs below:
   bool insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                           const QString& column1,
                                             const QString& index,const QString item);
   bool insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                           const QString& column1,
                                             const QString& index,const int item);
   bool insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                           const QString& column1,
                                             const QString& index,const bool item);
   bool insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                           const QString& column1,
                                             const QString& index,const double item);
   bool insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                           const QString& column1,
                                             const QString& index,const QByteArray item);
   bool insertItemIntoTableInColumnWhereColumn1EqualsIndex(const QString& table,const QString& column,
                                                           const QString& column1,
                                             const QString& index,const unsigned int item);


   //select * from table where condColumn=index;

   //returns a [QList<QVariant>],multiple records will be stored strictly under order;
   //FYI:I strongly recommand using loops with known columns numbers here;
   template<typename T>
   std::enable_if_t<std::is_convertible_v<T,std::string>,
   QList<QVariant>> selectFields(const QString& table,const QString& condColumn,T index);
   template<typename T>
   std::enable_if_t<std::is_same_v<T,QString>,
   QList<QVariant>> selectFields(const QString& table,const QString& condColumn,T index);
   template<typename T>
   std::enable_if_t<std::is_same_v<T,QByteArray>,
   QList<QVariant>> selectFields(const QString& table,const QString& condColumn,T index);
   template<typename T>
   std::enable_if_t<std::is_same_v<T,int>,
   QList<QVariant>> selectFields(const QString& table,const QString& condColumn,T index);
   template<typename T>
   std::enable_if_t<std::is_same_v<T,unsigned int>,
   QList<QVariant>> selectFields(const QString& table,const QString& condColumn,T index);
   template<typename T>
   std::enable_if_t<std::is_same_v<T,bool>,
   QList<QVariant>> selectFields(const QString& table,const QString& condColumn,T index);



   template<typename T>
   QList<QVariant> selectParticularField(const QString& table,const QString& targetColumn,
                                  const QString& condColumn,T index);


   //provide you with the ability to access database with raw sql string.
   std::shared_ptr<QSqlQuery> execRawString(const QString& str);

   const QString& getDatabaseName() const{return databaseName;};
protected:



    QSqlDatabase* db=nullptr;
    QSqlQuery* query=nullptr;
    QSqlError* error=nullptr;
    QSqlRecord* record=nullptr;




    std::vector<std::string> tables;
    QString databaseName;
    QString databaseType;


    void updateDatabaseInfo();



};


template<typename T>
void LibQSQL::createColumn(const QString& table,const QString& realName,T type)
{
    if constexpr(std::is_same_v<int,T>)
    {
        qDebug()<<"creating a int column ";
        QString queryString=QString("ALTER "
                                    "TABLE '%1' ADD COLUMN '%2' int").
                arg(QString(table)).arg(realName);

        query->exec(queryString);
        if(query->lastError().isValid())
        {
            qDebug()<<"error when creating column:"<<query->lastError();
        }

    }
    else if constexpr(std::is_same_v<unsigned int,T>)
    {
        qDebug()<<"creating an uInt column ";
        QString queryString=QString("ALTER "
                                    "TABLE '%1' ADD COLUMN '%2' UNSIGNED BIG INT").
                arg(QString(table)).arg(realName);

        query->exec(queryString);
        if(query->lastError().isValid())
        {
            qDebug()<<"error when creating column:"<<query->lastError();
        }
    }
    else if constexpr(std::is_same_v<bool,T>)
    {
        qDebug()<<"creating a bool column";
        QString queryString=QString("ALTER "
                                    "TABLE '%1' ADD COLUMN '%2' BOOLEAN").
                arg(QString(table)).arg(realName);

        query->exec(queryString);
        if(query->lastError().isValid())
        {
            qDebug()<<"error when creating column:"<<query->lastError();
        }
    }
    else if constexpr(std::is_same_v<std::string,T>||std::is_same_v<QString,T>
            ||std::is_convertible_v<T,std::string>)
    {

        qDebug()<<"creating a varchar column";

        QString queryString=QString("ALTER "
                                    "TABLE '%1' ADD COLUMN '%2' varchar").
                arg(QString(table)).arg(realName);

        query->exec(queryString);
        if(query->lastError().isValid())
        {
            qDebug()<<"error when creating column:"<<query->lastError();
        }

    }
}
//int a=1; bool b=1;QString c="r";
//initDatabase("testTable",)
template<typename field,typename ...fields>
void LibQSQL::initDatabase(const QString& firstTable,QVector<QString> names,field column,fields ...columns)
{
    if(db->tables().empty())
    {
        QString tmp=names.first();
        names.removeFirst();

        QString queryString;
        if constexpr(std::is_same_v<int,field>)
        {


            queryString=QString("create table %1(%2 int)").arg(firstTable).arg(tmp);
            query->exec(queryString);
            if(query->lastError().isValid())
            {
                qDebug()<<"error when creating column:"<<query->lastError();
            }

        }
        else if constexpr(std::is_same_v<unsigned int,field>)
        {
            queryString=QString("create table %1(%2 UNSIGNED BIG INT)").arg(firstTable).arg(tmp);
            query->exec(queryString);
            if(query->lastError().isValid())
            {
                qDebug()<<"error when creating column:"<<query->lastError();
            }
        }
        else if constexpr(std::is_same_v<bool,field>)
        {

            queryString=QString("create table %1(%2 BOOLEAN)").arg(firstTable).arg(tmp);
            query->exec(queryString);
            if(query->lastError().isValid())
            {
                qDebug()<<"error when creating column:"<<query->lastError();
            }

        }
        else if constexpr(std::is_same_v<std::string,field>||std::is_same_v<QString,field>
                ||std::is_convertible_v<field,std::string>)
        {

            qDebug()<<"creating a varchar column";

            queryString=QString("create table %1(%2 varchar)").arg(firstTable).arg(tmp);

            query->exec(queryString);
            if(query->lastError().isValid())
            {
                qDebug()<<"error when creating column:"<<query->lastError();
            }

        }
        if constexpr(sizeof...(columns)>0)
        initDatabase(firstTable,names,columns...);
    }
    else
    {
        if constexpr(sizeof...(columns)>0)
        {
            QString tmp=names.first();
            names.removeFirst();
            createColumn(firstTable,tmp,column);
            initDatabase(firstTable,names,columns...);
        }
        else
        {
            QString tmp=names.first();

            createColumn(firstTable,tmp,column);
        }
    }

};

template<typename field,typename ...fields>
void LibQSQL::createTable(const QString& tableName, QVector<QString> names,field column,fields ...columns)
{


    if(tables.size()==db->tables().size())
    {
        QString tmp=names.first();
        names.removeFirst();

        QString queryString;
        if constexpr(std::is_same_v<int,field>)
        {


            queryString=QString("create table %1(%2 int)").arg(tableName).arg(tmp);
            query->exec(queryString);
            if(query->lastError().isValid())
            {
                qDebug()<<"error when creating column:"<<query->lastError();
            }

        }
        else if constexpr(std::is_same_v<unsigned int,field>)
        {
            queryString=QString("create table %1(%2 UNSIGNED BIG INT)").arg(tableName).arg(tmp);
            query->exec(queryString);
            if(query->lastError().isValid())
            {
                qDebug()<<"error when creating column:"<<query->lastError();
            }
        }
        else if constexpr(std::is_same_v<bool,field>)
        {

            queryString=QString("create table %1(%2 BOOLEAN)").arg(tableName).arg(tmp);
            query->exec(queryString);
            if(query->lastError().isValid())
            {
                qDebug()<<"error when creating column:"<<query->lastError();
            }

        }
        else if constexpr(std::is_same_v<std::string,field>||std::is_same_v<QString,field>
                ||std::is_convertible_v<field,std::string>)
        {

            qDebug()<<"creating a  table";

            queryString=QString("create table %1(%2 varchar)").arg(tableName).arg(tmp);

            query->exec(queryString);
            if(query->lastError().isValid())
            {
                qDebug()<<"error when creating column:"<<query->lastError();
            }

        }

        if constexpr(sizeof...(columns)>0)
        createTable(tableName,names,columns...);

    }
    else
    {
        if constexpr(sizeof...(columns)>0)
        {
            QString tmp=names.first();
            names.removeFirst();
            createColumn(tableName,tmp,column);
            createTable(tableName,names,columns...);
        }
        else
        {
            QString tmp=names.first();
            createColumn(tableName,tmp,column);

        }
    }
    tables.push_back(tableName.toStdString());

}

template<typename T>

std::enable_if_t<std::is_convertible_v<T,std::string>,QList<QVariant>>
LibQSQL::selectFields(const QString& table,const QString& condColumn,T index)
{
     QString queryString=QString("select * from %1 where %2='%3'").arg(table)
             .arg(condColumn).arg(QString::fromStdString(std::string(index)));
     if(!query->exec(queryString))
     {
         qDebug()<<"error when select* in"<<query->lastQuery()<<"with "<<query->lastError();
         return QList<QVariant>();
     }

     *record=query->record();
     if(record->isEmpty())
     {
         return QList<QVariant>();
     }   
     QList<QVariant> rec;
     QStringList columnNames;
     for(int i=0;i<record->count();++i)
     {
         columnNames.append(record->field(i).name());
     }

     QVector<int> indexes;
     indexes.resize(record->count());

     for(int i=0;i<record->count();++i)
     {
         indexes[i]=record->indexOf(columnNames[i]);
     }


     while(query->next())
     {

         *record=query->record();
         for(auto i:indexes)
         {
             rec.append(query->value(i));
         }
     }
     return rec;

}

template<typename T>
std::enable_if_t<std::is_same_v<T,QString>,
QList<QVariant>> LibQSQL::selectFields(const QString& table,const QString& condColumn,T index)
{
    QString queryString=QString("select * from %1 where %2='%3'").arg(table)
            .arg(condColumn).arg(index);
    if(!query->exec(queryString))
    {
        qDebug()<<"error when select* in"<<query->lastQuery()<<"with "<<query->lastError();
        return QList<QVariant>();
    }

    *record=query->record();
    if(record->isEmpty())
    {
        return QList<QVariant>();
    }
    QList<QVariant> rec;
    QStringList columnNames;
    for(int i=0;i<record->count();++i)
    {
        columnNames.append(record->field(i).name());
    }

    QVector<int> indexes;
    indexes.resize(record->count());

    for(int i=0;i<record->count();++i)
    {
        indexes[i]=record->indexOf(columnNames[i]);
    }


    while(query->next())
    {

        *record=query->record();
        for(auto i:indexes)
        {
            rec.append(query->value(i));
        }
    }
    return rec;
}

//Is it really possible for someone to use this func?
template<typename T>
std::enable_if_t<std::is_same_v<T,QByteArray>,
QList<QVariant>> LibQSQL::selectFields(const QString& table,const QString& condColumn,T index)
{
    QString queryString=QString("select * from %1 where %2='%3'").arg(table)
            .arg(condColumn).arg(index);
    if(!query->exec(queryString))
    {
        qDebug()<<"error when select* in"<<query->lastQuery()<<"with "<<query->lastError();
        return QList<QVariant>();
    }

    *record=query->record();
    if(record->isEmpty())
    {
        return QList<QVariant>();
    }
    QList<QVariant> rec;
    QStringList columnNames;
    for(int i=0;i<record->count();++i)
    {
        columnNames.append(record->field(i).name());
    }

    QVector<int> indexes;
    indexes.resize(record->count());

    for(int i=0;i<record->count();++i)
    {
        indexes[i]=record->indexOf(columnNames[i]);
    }


    while(query->next())
    {

        *record=query->record();
        for(auto i:indexes)
        {
            rec.append(query->value(i));
        }
    }
    return rec;
}

template<typename T>
std::enable_if_t<std::is_same_v<T,int>,
QList<QVariant>> LibQSQL::selectFields(const QString& table,const QString& condColumn,T index)
{
    QString queryString=QString("select * from %1 where %2=%3").arg(table)
            .arg(condColumn).arg(index);
    if(!query->exec(queryString))
    {
        qDebug()<<"error when select* in"<<query->lastQuery()<<"with "<<query->lastError();
        return QList<QVariant>();
    }

    *record=query->record();
    if(record->isEmpty())
    {
        return QList<QVariant>();
    }
    QList<QVariant> rec;
    QStringList columnNames;
    for(int i=0;i<record->count();++i)
    {
        columnNames.append(record->field(i).name());
    }

    QVector<int> indexes;
    indexes.resize(record->count());

    for(int i=0;i<record->count();++i)
    {
        indexes[i]=record->indexOf(columnNames[i]);
    }


    while(query->next())
    {

        *record=query->record();
        for(auto i:indexes)
        {
            rec.append(query->value(i));
        }
    }
    return rec;
}

template<typename T>
std::enable_if_t<std::is_same_v<T,unsigned int>,
QList<QVariant>> LibQSQL::selectFields(const QString& table,const QString& condColumn,T index)
{
    QString queryString=QString("select * from %1 where %2=%3").arg(table)
            .arg(condColumn).arg(index);
    if(!query->exec(queryString))
    {
        qDebug()<<"error when select* in"<<query->lastQuery()<<"with "<<query->lastError();
        return QList<QVariant>();
    }

    *record=query->record();
    if(record->isEmpty())
    {
        return QList<QVariant>();
    }
    QList<QVariant> rec;
    QStringList columnNames;
    for(int i=0;i<record->count();++i)
    {
        columnNames.append(record->field(i).name());
    }

    QVector<int> indexes;
    indexes.resize(record->count());

    for(int i=0;i<record->count();++i)
    {
        indexes[i]=record->indexOf(columnNames[i]);
    }


    while(query->next())
    {

        *record=query->record();
        for(auto i:indexes)
        {
            rec.append(query->value(i));
        }
    }
    return rec;
}
template<typename T>
std::enable_if_t<std::is_same_v<T,bool>,
QList<QVariant>> LibQSQL::selectFields(const QString& table,const QString& condColumn,T index)
{
    QString queryString=QString("select * from %1 where %2=%3").arg(table)
            .arg(condColumn).arg(index);
    if(!query->exec(queryString))
    {
        qDebug()<<"error when select* in"<<query->lastQuery()<<"with "<<query->lastError();
        return QList<QVariant>();
    }

    *record=query->record();
    if(record->isEmpty())
    {
        return QList<QVariant>();
    }
    QList<QVariant> rec;
    QStringList columnNames;
    for(int i=0;i<record->count();++i)
    {
        columnNames.append(record->field(i).name());
    }

    QVector<int> indexes;
    indexes.resize(record->count());

    for(int i=0;i<record->count();++i)
    {
        indexes[i]=record->indexOf(columnNames[i]);
    }


    while(query->next())
    {

        *record=query->record();
        for(auto i:indexes)
        {
            rec.append(query->value(i));
        }
    }
    return rec;
}
template<typename T>
QList<QVariant> LibQSQL::selectParticularField(const QString& table,const QString& targetColumn,
                               const QString& condColumn,T index)
{
    if constexpr(std::is_same_v<int,T>)
    {

        QString queryString=QString("select %1 from %2 where %3=%4").
                arg(QString(targetColumn)).arg(table).arg(condColumn).arg(index);

        query->exec(queryString);
        if(query->lastError().isValid())
        {
            qDebug()<<"error when select :"<<query->lastQuery()<<"with "<<query->lastError();
        }

        *record=query->record();
        if(record->isEmpty())
        {
            return QList<QVariant>();
        }

         int index=record->indexOf(targetColumn);
        QList<QVariant> rec;
        while(query->next())
        {
           rec.append(query->value(index));
        }
        return rec;

    }
    else if constexpr(std::is_same_v<unsigned int,T>)
    {

        QString queryString=QString("select %1 from %2 where %3=%4").
                arg(QString(targetColumn)).arg(table).arg(condColumn).arg(index);

        query->exec(queryString);
        if(query->lastError().isValid())
        {
            qDebug()<<"error when select :"<<query->lastQuery()<<"with "<<query->lastError();
        }
        *record=query->record();
        if(record->isEmpty())
        {
            return QList<QVariant>();
        }

         int index=record->indexOf(targetColumn);
        QList<QVariant> rec;
        while(query->next())
        {
           rec.append(query->value(index));
        }
        return rec;
    }
    else if constexpr(std::is_same_v<bool,T>)
    {
        QString queryString=QString("select %1 from %2 where %3=%4").
                arg(QString(targetColumn)).arg(table).arg(condColumn).arg(index);

        query->exec(queryString);
        if(query->lastError().isValid())
        {
            qDebug()<<"error when select :"<<query->lastQuery()<<"with "<<query->lastError();
        }
        *record=query->record();
        if(record->isEmpty())
        {
            return QList<QVariant>();
        }


         int index=record->indexOf(targetColumn);
        QList<QVariant> rec;
        while(query->next())
        {
           rec.append(query->value(index));
        }
        return rec;
    }
    else if constexpr(std::is_same_v<std::string,T>||std::is_same_v<QString,T>
            ||std::is_convertible_v<T,std::string>)
    {
        QString buf;
        if constexpr(std::is_same_v<std::string,T>)
        {
            buf=QString::fromStdString(index);
        }
        else
        {
            buf=QString(index);
        }

        QString queryString=QString("select %1 from %2 where %3='%4'").
                arg(QString(targetColumn)).arg(table).arg(condColumn).arg(QString(buf));

        query->exec(queryString);
        if(query->lastError().isValid())
        {
            qDebug()<<"error when select :"<<query->lastQuery()<<"with "<<query->lastError();
        }
        *record=query->record();
        if(record->isEmpty())
        {
            return QList<QVariant>();
        }

        int index=record->indexOf(targetColumn);
        QList<QVariant> rec;
        while(query->next())
        {
           rec.append(query->value(index));
        }
        return rec;

    }
}

_end_LL
#endif // LIBQSQL_H
