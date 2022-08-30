#include "libqsql.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{

   QCoreApplication a(argc, argv);

   L_library::LibQSQL tmp("QSQLITE","mydata.db");

   // int inter=0;
   // bool b=true;
   QString st="real1";
   QVector<QString> names;
   names<<"test1"<<"test2"<<"test3";

   QList<QString> tmpList1;
   tmpList1<<"test1"<<"test2";
   QList<QString> tmpList2;
   tmpList2<<"12"<<"true";
   tmpList1.clear();
   tmpList1<<"test3"<<"strColumn";

   qDebug()<<tmp.selectFields("test","strColumn","true");
   qDebug()<<tmp.selectParticularField("test","test1","test3","real");
   qDebug()<<tmp.selectParticularField("test","test3","test1",13);
   tmp.printColumns("test");



  // tmp.createTable("testTable1_STR",names,st,st,st);

    return 0;
}
