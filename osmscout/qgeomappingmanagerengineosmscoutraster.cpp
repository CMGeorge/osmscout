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

#include "qgeomappingmanagerengineosmscoutraster.h"

#include "qgeotilefetcherosmscout.h"
#include <QtLocation/private/qgeocameracapabilities_p.h>
#include <QtLocation/private/qgeomaptype_p.h>
#include <QtLocation/private/qgeotiledmapdata_p.h>

QGeoMappingManagerEngineOsmScoutRaster::QGeoMappingManagerEngineOsmScoutRaster(const QVariantMap &parameters,
                                       QGeoServiceProvider::Error *error, QString *errorString) :
    QGeoTiledMappingManagerEngine()
{
    QGeoCameraCapabilities cameraCaps;
    cameraCaps.setMinimumZoomLevel(0.0);
    cameraCaps.setMaximumZoomLevel(19.0);
    setCameraCapabilities(cameraCaps);

    setTileSize(QSize(256, 256));
    setCacheHint(QGeoTiledMappingManagerEngine::MemoryCache);

    QList<QGeoMapType> mapTypes;
    mapTypes << QGeoMapType(QGeoMapType::StreetMap, tr("Day Street Map"), tr("OpenStreetMap street map"), false, false, OsmScoutDefaultDayMap);
    setSupportedMapTypes(mapTypes);

    QGeoTileFetcherOsmScout *tileFetcher = new QGeoTileFetcherOsmScout(this);
    setTileFetcher(tileFetcher);

    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

QGeoMappingManagerEngineOsmScoutRaster::~QGeoMappingManagerEngineOsmScoutRaster()
{

}

QGeoMapData *QGeoMappingManagerEngineOsmScoutRaster::createMapData()
{
    return new QGeoTiledMapData(this, 0);
}
