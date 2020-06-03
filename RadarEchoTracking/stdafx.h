// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <QtCore>
#include <QtGui>
#include <QtXml/QtXml>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDb/WriteFile>
#include <osgSim/ColorRange>
#include <osgSim/ScalarBar>
#include <AdapterWidget.h>
#include <MainControlFrame.h>
#include <Map.h>
#include <GeosOnTheOsg.h>
#include <DrawGeometryManipulator.h>
#include <DrawRect.h>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_multi_barchart.h>
#include <qwt_plot_renderer.h>

#include "../NetCDFDataProvider2/DataObtainInterface.h"
#include "../NetCDFDataProvider2/ControlorInterface.h"
#include "../NetCDFDataProvider2/SetCubesInterface.h"
#include "../PaoDianProvider/PaoDinaControlerInterface.h"
#include "../FlightPathProvider/FlightPathControler.h"
#include "../QwtRasterPolt/MemoryData.h"
// TODO: reference additional headers your program requires here
