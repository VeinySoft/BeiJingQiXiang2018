/****************************************************************************
** Meta object code from reading C++ file 'QwtPlotDialogImps.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../QwtPlotDialogImps.h"
#include "ui_TabPlot.h"
#include "../DistroScaleDraw.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QwtPlotDialogImps.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwtPlotDialogImps[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      46,   18,   18,   18, 0x0a,
      72,   66,   18,   18, 0x0a,
     105,   66,   18,   18, 0x0a,
     153,  141,   18,   18, 0x0a,
     191,  141,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QwtPlotDialogImps[] = {
    "QwtPlotDialogImps\0\0slot_CopyToClipBorad(bool)\0"
    "slot_HideMark(bool)\0index\0"
    "slot_clip_activated(QModelIndex)\0"
    "slot_Section_activated(QModelIndex)\0"
    "currentText\0slot_clip_currentTextChanged(QString)\0"
    "slot_Section_currentTextChanged(QString)\0"
};

void QwtPlotDialogImps::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwtPlotDialogImps *_t = static_cast<QwtPlotDialogImps *>(_o);
        switch (_id) {
        case 0: _t->slot_CopyToClipBorad((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->slot_HideMark((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slot_clip_activated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->slot_Section_activated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->slot_clip_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->slot_Section_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwtPlotDialogImps::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwtPlotDialogImps::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QwtPlotDialogImps,
      qt_meta_data_QwtPlotDialogImps, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwtPlotDialogImps::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwtPlotDialogImps::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwtPlotDialogImps::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtPlotDialogImps))
        return static_cast<void*>(const_cast< QwtPlotDialogImps*>(this));
    return QObject::qt_metacast(_clname);
}

int QwtPlotDialogImps::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
