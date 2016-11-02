/* MIT License
*
* Copyright (c) 2016 Alexey Edelev
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef QGEOMAPPINGMANAGERENGINEOSMSCOUTRASTER_H
#define QGEOMAPPINGMANAGERENGINEOSMSCOUTRASTER_H

#include <QtLocation/QGeoServiceProvider>

#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>

QT_BEGIN_NAMESPACE

class QGeoMappingManagerEngineOsmScoutRaster : public QGeoTiledMappingManagerEngine
{
public:
    typedef enum {
        OsmScoutDefaultDayMap = 1
    } OsmScoutMapIds;
    QGeoMappingManagerEngineOsmScoutRaster(const QVariantMap &parameters,
                                           QGeoServiceProvider::Error *error, QString *errorString);
    ~QGeoMappingManagerEngineOsmScoutRaster();

    QGeoMapData *createMapData();
};
QT_END_NAMESPACE

#endif // QGEOMAPPINGMANAGERENGINEOSMSCOUTRASTER_H
