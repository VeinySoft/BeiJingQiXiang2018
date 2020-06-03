/****************************************************************************
** Meta object code from reading C++ file 'ModeSettingDialogImps.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../ModeSettingDialogImps.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModeSettingDialogImps.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModeSettingDialogImps[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      42,   22,   22,   22, 0x0a,
      64,   22,   22,   22, 0x0a,
      86,   22,   22,   22, 0x0a,
     103,   22,   22,   22, 0x0a,
     124,   22,   22,   22, 0x0a,
     149,   22,   22,   22, 0x0a,
     175,   22,   22,   22, 0x0a,
     201,   22,   22,   22, 0x0a,
     227,   22,   22,   22, 0x0a,
     236,   22,   22,   22, 0x0a,
     257,   22,   22,   22, 0x0a,
     283,  277,   22,   22, 0x0a,
     303,  277,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ModeSettingDialogImps[] = {
    "ModeSettingDialogImps\0\0slot_AddCube(bool)\0"
    "slot_ModifyCube(bool)\0slot_DeleteCube(bool)\0"
    "slot_Apply(bool)\0slot_addToList(bool)\0"
    "slot_setPathButton(bool)\0"
    "slot_setPathButton2(bool)\0"
    "slot_setPathButton3(bool)\0"
    "slot_RemoveFromList(bool)\0accept()\0"
    "slot_FirstTime(bool)\0slot_LastTime(bool)\0"
    "index\0slot_activated(int)\0"
    "slot_buttonClicked(int)\0"
};

void ModeSettingDialogImps::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModeSettingDialogImps *_t = static_cast<ModeSettingDialogImps *>(_o);
        switch (_id) {
        case 0: _t->slot_AddCube((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->slot_ModifyCube((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slot_DeleteCube((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->slot_Apply((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slot_addToList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->slot_setPathButton((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->slot_setPathButton2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->slot_setPathButton3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->slot_RemoveFromList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->accept(); break;
        case 10: _t->slot_FirstTime((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->slot_LastTime((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->slot_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->slot_buttonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModeSettingDialogImps::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModeSettingDialogImps::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ModeSettingDialogImps,
      qt_meta_data_ModeSettingDialogImps, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModeSettingDialogImps::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModeSettingDialogImps::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModeSettingDialogImps::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModeSettingDialogImps))
        return static_cast<void*>(const_cast< ModeSettingDialogImps*>(this));
    return QDialog::qt_metacast(_clname);
}

int ModeSettingDialogImps::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
