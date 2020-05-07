/****************************************************************************
** Meta object code from reading C++ file 'myscene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "myscene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyScene_t {
    QByteArrayData data[12];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyScene_t qt_meta_stringdata_MyScene = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MyScene"
QT_MOC_LITERAL(1, 8, 16), // "infoZmeneneUlica"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "ulicaClass*"
QT_MOC_LITERAL(4, 38, 5), // "ulica"
QT_MOC_LITERAL(5, 44, 19), // "infoZmeneneZastavka"
QT_MOC_LITERAL(6, 64, 14), // "zastavkaClass*"
QT_MOC_LITERAL(7, 79, 8), // "zastavka"
QT_MOC_LITERAL(8, 88, 18), // "infoZmeneneAutobus"
QT_MOC_LITERAL(9, 107, 13), // "autobusClass*"
QT_MOC_LITERAL(10, 121, 7), // "autobus"
QT_MOC_LITERAL(11, 129, 11) // "zobrazModal"

    },
    "MyScene\0infoZmeneneUlica\0\0ulicaClass*\0"
    "ulica\0infoZmeneneZastavka\0zastavkaClass*\0"
    "zastavka\0infoZmeneneAutobus\0autobusClass*\0"
    "autobus\0zobrazModal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyScene[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       8,    1,   40,    2, 0x06 /* Public */,
      11,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,    2,

       0        // eod
};

void MyScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyScene *_t = static_cast<MyScene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->infoZmeneneUlica((*reinterpret_cast< ulicaClass*(*)>(_a[1]))); break;
        case 1: _t->infoZmeneneZastavka((*reinterpret_cast< zastavkaClass*(*)>(_a[1]))); break;
        case 2: _t->infoZmeneneAutobus((*reinterpret_cast< autobusClass*(*)>(_a[1]))); break;
        case 3: _t->zobrazModal((*reinterpret_cast< autobusClass*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ulicaClass* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< autobusClass* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< autobusClass* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyScene::*_t)(ulicaClass * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyScene::infoZmeneneUlica)) {
                *result = 0;
            }
        }
        {
            typedef void (MyScene::*_t)(zastavkaClass * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyScene::infoZmeneneZastavka)) {
                *result = 1;
            }
        }
        {
            typedef void (MyScene::*_t)(autobusClass * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyScene::infoZmeneneAutobus)) {
                *result = 2;
            }
        }
        {
            typedef void (MyScene::*_t)(autobusClass * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyScene::zobrazModal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject MyScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_MyScene.data,
      qt_meta_data_MyScene,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyScene::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyScene.stringdata0))
        return static_cast<void*>(const_cast< MyScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int MyScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MyScene::infoZmeneneUlica(ulicaClass * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyScene::infoZmeneneZastavka(zastavkaClass * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyScene::infoZmeneneAutobus(autobusClass * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MyScene::zobrazModal(autobusClass * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
