/****************************************************************************
** Meta object code from reading C++ file 'CubeSettingsDialogImps.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../CubeSettingsDialogImps.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CubeSettingsDialogImps.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CubeSettingsDialogImps[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   24,   23,   23, 0x0a,
      54,   23,   23,   23, 0x0a,
      80,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CubeSettingsDialogImps[] = {
    "CubeSettingsDialogImps\0\0index\0"
    "slot_buttonClicked(int)\0"
    "slot_AddValueButton(bool)\0"
    "slot_DelValueButton(bool)\0"
};

void CubeSettingsDialogImps::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CubeSettingsDialogImps *_t = static_cast<CubeSettingsDialogImps *>(_o);
        switch (_id) {
        case 0: _t->slot_buttonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slot_AddValueButton((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slot_DelValueButton((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CubeSettingsDialogImps::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CubeSettingsDialogImps::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CubeSettingsDialogImps,
      qt_meta_data_CubeSettingsDialogImps, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CubeSettingsDialogImps::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CubeSettingsDialogImps::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CubeSettingsDialogImps::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CubeSettingsDialogImps))
        return static_cast<void*>(const_cast< CubeSettingsDialogImps*>(this));
    return QDialog::qt_metacast(_clname);
}

int CubeSettingsDialogImps::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
