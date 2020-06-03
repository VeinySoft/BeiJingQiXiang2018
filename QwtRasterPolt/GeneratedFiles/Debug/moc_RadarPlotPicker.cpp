/****************************************************************************
** Meta object code from reading C++ file 'RadarPlotPicker.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../RadarPlotPicker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadarPlotPicker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RadarPlotPicker[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_RadarPlotPicker[] = {
    "RadarPlotPicker\0\0posF\0CurrentPoint(QPointF)\0"
};

void RadarPlotPicker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RadarPlotPicker *_t = static_cast<RadarPlotPicker *>(_o);
        switch (_id) {
        case 0: _t->CurrentPoint((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RadarPlotPicker::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RadarPlotPicker::staticMetaObject = {
    { &QwtPlotPicker::staticMetaObject, qt_meta_stringdata_RadarPlotPicker,
      qt_meta_data_RadarPlotPicker, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RadarPlotPicker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RadarPlotPicker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RadarPlotPicker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RadarPlotPicker))
        return static_cast<void*>(const_cast< RadarPlotPicker*>(this));
    return QwtPlotPicker::qt_metacast(_clname);
}

int RadarPlotPicker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlotPicker::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void RadarPlotPicker::CurrentPoint(const QPointF & _t1)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< RadarPlotPicker *>(this), &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
