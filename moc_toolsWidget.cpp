/****************************************************************************
** Meta object code from reading C++ file 'toolsWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "toolsWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ToolsWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      35,   12,   12,   12, 0x0a,
      55,   12,   12,   12, 0x0a,
      81,   12,   12,   12, 0x0a,
      99,   12,   12,   12, 0x0a,
     119,   12,   12,   12, 0x0a,
     140,   12,   12,   12, 0x0a,
     164,   12,   12,   12, 0x0a,
     190,   12,   12,   12, 0x0a,
     204,   12,   12,   12, 0x0a,
     218,   12,   12,   12, 0x0a,
     232,   12,   12,   12, 0x0a,
     246,   12,   12,   12, 0x0a,
     264,   12,   12,   12, 0x0a,
     285,   12,   12,   12, 0x0a,
     305,   12,   12,   12, 0x0a,
     320,   12,   12,   12, 0x0a,
     334,   12,   12,   12, 0x0a,
     355,   12,   12,   12, 0x0a,
     371,   12,   12,   12, 0x0a,
     389,   12,   12,   12, 0x0a,
     410,   12,   12,   12, 0x0a,
     431,   12,   12,   12, 0x0a,
     446,   12,   12,   12, 0x0a,
     461,   12,   12,   12, 0x0a,
     479,   12,   12,   12, 0x0a,
     497,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ToolsWidget[] = {
    "ToolsWidget\0\0createButtonClicked()\0"
    "editButtonClicked()\0curveTypeChanged(QString)\0"
    "penColorClicked()\0brushColorClicked()\0"
    "penWidthChanged(int)\0penColorChanged(QColor)\0"
    "brushColorChanged(QColor)\0playClicked()\0"
    "stopClicked()\0prevClicked()\0nextClicked()\0"
    "frameChanged(int)\0addPtBeforeClicked()\0"
    "addPtAfterClicked()\0delPtClicked()\0"
    "sizeChanged()\0nbFramesChanged(int)\0"
    "fpsChanged(int)\0kfModePtClicked()\0"
    "kfModeCurveClicked()\0kfModeSceneClicked()\0"
    "kfAddClicked()\0kfDelClicked()\0"
    "kfNextPtClicked()\0kfPrevPtClicked()\0"
    "kfApplyTypeClicked()\0"
};

void ToolsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ToolsWidget *_t = static_cast<ToolsWidget *>(_o);
        switch (_id) {
        case 0: _t->createButtonClicked(); break;
        case 1: _t->editButtonClicked(); break;
        case 2: _t->curveTypeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->penColorClicked(); break;
        case 4: _t->brushColorClicked(); break;
        case 5: _t->penWidthChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->penColorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 7: _t->brushColorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 8: _t->playClicked(); break;
        case 9: _t->stopClicked(); break;
        case 10: _t->prevClicked(); break;
        case 11: _t->nextClicked(); break;
        case 12: _t->frameChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->addPtBeforeClicked(); break;
        case 14: _t->addPtAfterClicked(); break;
        case 15: _t->delPtClicked(); break;
        case 16: _t->sizeChanged(); break;
        case 17: _t->nbFramesChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->fpsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->kfModePtClicked(); break;
        case 20: _t->kfModeCurveClicked(); break;
        case 21: _t->kfModeSceneClicked(); break;
        case 22: _t->kfAddClicked(); break;
        case 23: _t->kfDelClicked(); break;
        case 24: _t->kfNextPtClicked(); break;
        case 25: _t->kfPrevPtClicked(); break;
        case 26: _t->kfApplyTypeClicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ToolsWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ToolsWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ToolsWidget,
      qt_meta_data_ToolsWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ToolsWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ToolsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ToolsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToolsWidget))
        return static_cast<void*>(const_cast< ToolsWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ToolsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
