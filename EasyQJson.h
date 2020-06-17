#ifndef EASYQJSON_H
#define EASYQJSON_H
#include<QFile>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonValue>

namespace DJY{
    class EasyQJson
    {
    public:
        //读取json文件并且返回Json对象
        QJsonObject readJsonFileReturnObject(QString filepath)
        {
            QJsonObject rootObj = {};
            QFile file(filepath);
            if(!file.open(QIODevice::ReadWrite))//打开文件
            {
                return rootObj;
            }
            //throw std::logical_error();

            QByteArray all = file.readAll();
            file.close();
            QJsonParseError json_error;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(all,&json_error)); //读取json
            rootObj = jsonDoc.object();
            return rootObj;
        }
        //读取QJsonObject返回QString
        QString readObjectReturnQString(QJsonObject object)
        {
            QString result = QString(QJsonDocument(object).toJson());
            return result;
        }

        //读取QString返回QJsonObject
        QJsonObject ReadQStringReturnObject(const QString jsonString){
            QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
            if( jsonDocument.isNull() ){
                return {};
            }
            QJsonObject jsonObject = jsonDocument.object();
            return jsonObject;
        }

        //提供JsonObject和键值，返回Json数组
        QJsonArray readJsonObjectReturnArray(QJsonObject object, QString jsonkey)
        {
            QJsonValue value = object.value(jsonkey);
            QJsonArray array = value.toArray();
            return array;
        }
        //判断Json数组有没有对应的键
        bool JsonArrayContain(QJsonArray array, QString jsonObject, QString target)
        {
            //传入了array ，jsonobject为要找到的json对象键名，target为这个对象的值
            for(int i = 0;i<array.size();i++)
            {
                QJsonValue arrayValue = array.at(i);
                QString projectname = arrayValue[jsonObject].toString();

                if(projectname == target)
                {

                    return true;
                }
            }
            return false;
        }

        //传入键，从Json数组读取其中的Json数组 (格式是一个Json数组，其中包含Json对象和另一个Json数组)
        //target是要找的键，key是数组中的键
        QJsonArray readArrayReturnArray(QJsonArray array,QString target,QString key,QString arrayKey)
        {
            QJsonValue value;
            QJsonArray thisArray = {};
            for(int i=0;i<array.size();i++)
            {
                value = array.at(i);
                if(value[target].toString() == key)
                {
                    thisArray = value[arrayKey].toArray();
                }
            }
            return thisArray;
        }

        //传入键，从Json数组读取其中的Json对象 (格式是一个Json数组，其中包含Json对象和另一个Json数组)
        //target是要找的键，key是数组中的键
        QJsonObject readArrayReturnObject(QJsonArray array,QString target,QString key,QString arrayKey)
        {
            QJsonValue value;
            QJsonObject thisObject = {};
            for(int i=0;i<array.size();i++)
            {
                value = array.at(i);
                if(value[target].toString() == key)
                {
                    thisObject = value[arrayKey].toObject();
                }
            }
            return thisObject;
        }

        bool writeObject2JsonFile(QJsonObject object,QString filename)
        {
            QFile file(filename);
            if(!file.open(QIODevice::WriteOnly))
            {
                return false;
            }
            QJsonDocument jsondocu(object);
            file.write(jsondocu.toJson());
            file.close();
            return true;
        }

    };



}






#endif // EASYQJSON_H
