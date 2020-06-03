/****************************************************************************
** Meta object code from reading C++ file 'PaoDianListDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../PaoDianListDialog.h"
#include "ui_PaoDianDockWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PaoDianListDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PaoDianListDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   19,   18,   18, 0x08,
      56,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PaoDianListDialog[] = {
    "PaoDianListDialog\0\0item\0"
    "treeItemChanged(QStandardItem*)\0"
    "slot_accepted()\0"
};

void PaoDianListDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PaoDianListDialog *_t = static_cast<PaoDianListDialog *>(_o);
        switch (_id) {
        case 0: _t->treeItemChanged((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        case 1: _t->slot_accepted(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PaoDianListDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PaoDianListDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PaoDianListDialog,
      qt_meta_data_PaoDianListDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PaoDianListDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PaoDianListDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PaoDianListDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PaoDianListDialog))
        return static_cast<void*>(const_cast< PaoDianListDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int PaoDianListDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
