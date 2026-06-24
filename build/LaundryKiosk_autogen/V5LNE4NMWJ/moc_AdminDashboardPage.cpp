/****************************************************************************
** Meta object code from reading C++ file 'AdminDashboardPage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/pages/AdminDashboardPage.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AdminDashboardPage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AdminDashboardPage_t {
    const uint offsetsAndSize[12];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_AdminDashboardPage_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_AdminDashboardPage_t qt_meta_stringdata_AdminDashboardPage = {
    {
QT_MOC_LITERAL(0, 18), // "AdminDashboardPage"
QT_MOC_LITERAL(19, 13), // "backRequested"
QT_MOC_LITERAL(33, 0), // ""
QT_MOC_LITERAL(34, 18), // "toggleMachineState"
QT_MOC_LITERAL(53, 2), // "id"
QT_MOC_LITERAL(56, 17) // "resetMachineState"

    },
    "AdminDashboardPage\0backRequested\0\0"
    "toggleMachineState\0id\0resetMachineState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AdminDashboardPage[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    1 /* Public */,
       3,    1,   33,    2, 0x06,    2 /* Public */,
       5,    1,   36,    2, 0x06,    4 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void AdminDashboardPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AdminDashboardPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->backRequested(); break;
        case 1: _t->toggleMachineState((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->resetMachineState((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AdminDashboardPage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AdminDashboardPage::backRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AdminDashboardPage::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AdminDashboardPage::toggleMachineState)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AdminDashboardPage::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AdminDashboardPage::resetMachineState)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AdminDashboardPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_AdminDashboardPage.offsetsAndSize,
    qt_meta_data_AdminDashboardPage,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_AdminDashboardPage_t
, QtPrivate::TypeAndForceComplete<AdminDashboardPage, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>



>,
    nullptr
} };


const QMetaObject *AdminDashboardPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AdminDashboardPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AdminDashboardPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AdminDashboardPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void AdminDashboardPage::backRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AdminDashboardPage::toggleMachineState(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AdminDashboardPage::resetMachineState(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
