/****************************************************************************
** Meta object code from reading C++ file 'v4l2.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "v4l2.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'v4l2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_V4l2_t {
    QByteArrayData data[12];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_V4l2_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_V4l2_t qt_meta_stringdata_V4l2 = {
    {
QT_MOC_LITERAL(0, 0, 4), // "V4l2"
QT_MOC_LITERAL(1, 5, 16), // "backto_main_page"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 17), // "start_photo_album"
QT_MOC_LITERAL(4, 41, 15), // "show_video_page"
QT_MOC_LITERAL(5, 57, 20), // "set_open_button_text"
QT_MOC_LITERAL(6, 78, 5), // "start"
QT_MOC_LITERAL(7, 84, 10), // "show_video"
QT_MOC_LITERAL(8, 95, 5), // "image"
QT_MOC_LITERAL(9, 101, 17), // "show_currentphoto"
QT_MOC_LITERAL(10, 119, 23), // "on_photo_button_clicked"
QT_MOC_LITERAL(11, 143, 29) // "on_back_tomain_button_clicked"

    },
    "V4l2\0backto_main_page\0\0start_photo_album\0"
    "show_video_page\0set_open_button_text\0"
    "start\0show_video\0image\0show_currentphoto\0"
    "on_photo_button_clicked\0"
    "on_back_tomain_button_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_V4l2[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    1,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   58,    2, 0x0a /* Public */,
       5,    1,   59,    2, 0x0a /* Public */,
       7,    1,   62,    2, 0x0a /* Public */,
       9,    0,   65,    2, 0x0a /* Public */,
      10,    0,   66,    2, 0x08 /* Private */,
      11,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::QImage,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void V4l2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<V4l2 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backto_main_page(); break;
        case 1: _t->start_photo_album((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->show_video_page(); break;
        case 3: _t->set_open_button_text((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->show_video((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 5: _t->show_currentphoto(); break;
        case 6: _t->on_photo_button_clicked(); break;
        case 7: _t->on_back_tomain_button_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (V4l2::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&V4l2::backto_main_page)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (V4l2::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&V4l2::start_photo_album)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject V4l2::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_V4l2.data,
    qt_meta_data_V4l2,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *V4l2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *V4l2::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_V4l2.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int V4l2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void V4l2::backto_main_page()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void V4l2::start_photo_album(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
