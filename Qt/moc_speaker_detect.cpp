/****************************************************************************
** Meta object code from reading C++ file 'speaker_detect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "speaker_detect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'speaker_detect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Testing_t {
    QByteArrayData data[4];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Testing_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Testing_t qt_meta_stringdata_Testing = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Testing"
QT_MOC_LITERAL(1, 8, 16), // "updateTestResult"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4) // "info"

    },
    "Testing\0updateTestResult\0\0info"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Testing[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void Testing::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Testing *_t = static_cast<Testing *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateTestResult((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Testing::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Testing::updateTestResult)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Testing::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Testing.data,
      qt_meta_data_Testing,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Testing::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Testing::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Testing.stringdata0))
        return static_cast<void*>(const_cast< Testing*>(this));
    return QThread::qt_metacast(_clname);
}

int Testing::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Testing::updateTestResult(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_recognitionWidget_t {
    QByteArrayData data[10];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_recognitionWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_recognitionWidget_t qt_meta_stringdata_recognitionWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "recognitionWidget"
QT_MOC_LITERAL(1, 18, 3), // "add"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 3), // "del"
QT_MOC_LITERAL(4, 27, 12), // "startTesting"
QT_MOC_LITERAL(5, 40, 2), // "ok"
QT_MOC_LITERAL(6, 43, 6), // "ok_del"
QT_MOC_LITERAL(7, 50, 13), // "initializeSil"
QT_MOC_LITERAL(8, 64, 18), // "onUpdateTestResult"
QT_MOC_LITERAL(9, 83, 4) // "info"

    },
    "recognitionWidget\0add\0\0del\0startTesting\0"
    "ok\0ok_del\0initializeSil\0onUpdateTestResult\0"
    "info"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_recognitionWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    1,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,

       0        // eod
};

void recognitionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        recognitionWidget *_t = static_cast<recognitionWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->add(); break;
        case 1: _t->del(); break;
        case 2: _t->startTesting(); break;
        case 3: _t->ok(); break;
        case 4: _t->ok_del(); break;
        case 5: _t->initializeSil(); break;
        case 6: _t->onUpdateTestResult((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject recognitionWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_recognitionWidget.data,
      qt_meta_data_recognitionWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *recognitionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *recognitionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_recognitionWidget.stringdata0))
        return static_cast<void*>(const_cast< recognitionWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int recognitionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
