/****************************************************************************
** Meta object code from reading C++ file 'HomePage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/pages/HomePage.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HomePage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HomePage_t {
    const uint offsetsAndSize[10];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_HomePage_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_HomePage_t qt_meta_stringdata_HomePage = {
    {
QT_MOC_LITERAL(0, 8), // "HomePage"
QT_MOC_LITERAL(9, 15), // "machineSelected"
QT_MOC_LITERAL(25, 0), // ""
QT_MOC_LITERAL(26, 9), // "machineId"
QT_MOC_LITERAL(36, 14) // "adminRequested"

    },
    "HomePage\0machineSelected\0\0machineId\0"
    "adminRequested"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HomePage[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x06,    1 /* Public */,
       4,    0,   29,    2, 0x06,    3 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

       0        // eod
};

void HomePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HomePage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->machineSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->adminRequested(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HomePage::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePage::machineSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HomePage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePage::adminRequested)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject HomePage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_HomePage.offsetsAndSize,
    qt_meta_data_HomePage,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_HomePage_t
, QtPrivate::TypeAndForceComplete<HomePage, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>



>,
    nullptr
} };


const QMetaObject *HomePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HomePage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HomePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void HomePage::machineSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HomePage::adminRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
