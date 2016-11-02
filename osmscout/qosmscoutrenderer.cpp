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

#include "qosmscoutrenderer.h"

#include <QtLocation/private/qgeotilespec_p.h>
#include <osmscout/MapPainterQt.h>
#include <QtGui/QPainter>
#include <QtCore/QBuffer>
#include <QFile>
#include <math.h>
#include <QDebug>

QOsmScoutRendererPrivate::QOsmScoutRendererPrivate():
    databaseParameter()
  ,database(new osmscout::Database(databaseParameter))
  ,mapService(new osmscout::MapService(database))
  ,styleConfig(0)
  ,stylePath("stylesheets/standard.oss")
  ,mapPath("maps/")
  ,databaseOpen(false){}


QOsmScoutRenderer::QOsmScoutRenderer(QObject *parent) :
    QObject(parent)
  ,m_pixmap(new QPixmap(256, 256))//TODO: replace hardcoded number with needle tile size
  ,m_qtPainter(new QPainter(m_pixmap))
  ,d(new QOsmScoutRendererPrivate())
{
    d->mapPainter = new osmscout::MapPainterQt();
    d->databaseOpen = d->database->Open(d->mapPath.toStdString().c_str());
    if (!d->databaseOpen) {
        qCritical() << "QOsmScoutRenderer: Cannot open osm scout database";
    }
    else
    {
        d->styleConfig = new osmscout::StyleConfig(d->database->GetTypeConfig());
        if (!osmscout::LoadStyleConfig(d->stylePath.toStdString().c_str(), *(d->styleConfig))) {
            qCritical() << "QOsmScoutRenderer: Cannot open osm scout style configuration";
        }
    }
}

QOsmScoutRenderer::~QOsmScoutRenderer()
{
    delete m_qtPainter;
    delete m_pixmap;
}

void QOsmScoutRenderer::render(const QGeoTileSpec &spec)
{
    qreal tilesCount = pow(2.0, spec.zoom());
    qreal lonMin = (float)spec.x() / tilesCount * 360.0 - 180.0;
    qreal lonMax = (float)(spec.x() + 1)/ tilesCount * 360.0 - 180.0;
    qreal n = M_PI - 2.0 * M_PI * (float)(spec.y()+1) / tilesCount;
    qreal latMin = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    n = M_PI - 2.0 * M_PI * (float)spec.y() / tilesCount;
    qreal latMax = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));

    QByteArray tileBuffer;
    QBuffer buffer(&tileBuffer);
    osmscout::MapPainterQt* scoutPainter = dynamic_cast<osmscout::MapPainterQt*>(d->mapPainter);
    osmscout::MercatorProjection  projection;
    osmscout::TypeSet              nodeTypes;
    std::vector<osmscout::TypeSet> wayTypes;
    osmscout::TypeSet              areaTypes;

    if(!buffer.open(QIODevice::WriteOnly))
    {
        qCritical() << "QOsmScoutRenderer: Cannot open temporary QBuffer to store tile image";
        tileReady(QByteArray(), QGeoTiledMapReply::UnknownError, spec);
        return;
    }

    if(latMax == latMin || lonMax == lonMin)
    {
        qCritical() << "QOsmScoutRenderer: Tile top-left edge is equal to right-bottom";
        tileReady(QByteArray(), QGeoTiledMapReply::ParseError, spec);
        return;
    }

    if(!d->databaseOpen)
    {
        qCritical() << "QOsmScoutRenderer: Osm scout database is not opened";
        tileReady(QByteArray(), QGeoTiledMapReply::CommunicationError, spec);
        return;
    }

    if(!scoutPainter)
    {
        qCritical() << "QOsmScoutRenderer: Scout painer is not initialized";
        tileReady(QByteArray(), QGeoTiledMapReply::UnknownError, spec);
        return;
    }

    projection.Set(lonMin,
                   latMin,
                   lonMax,
                   latMax,
                   tilesCount,
                   256);//TODO: replace hardcoded number with needle tile size


    d->styleConfig->GetNodeTypesWithMaxMag(projection.GetMagnification(),
                                       nodeTypes);

    d->styleConfig->GetWayTypesByPrioWithMaxMag(projection.GetMagnification(),
                                            wayTypes);

    d->styleConfig->GetAreaTypesWithMaxMag(projection.GetMagnification(),
                                       areaTypes);

    d->mapService->GetObjects(nodeTypes,
                           wayTypes,
                           areaTypes,
                           projection.GetLonMin(),
                           projection.GetLatMin(),
                           projection.GetLonMax(),
                           projection.GetLatMax(),
                           projection.GetMagnification(),
                           d->searchParameter,
                           d->mapData.nodes,
                           d->mapData.ways,
                           d->mapData.areas);
    if(!scoutPainter->DrawMap(*(d->styleConfig),
                          projection,
                          d->mapParameter,
                          d->mapData,
                          m_qtPainter))
    {
        qCritical() << "QOsmScoutRenderer: Unknown error while drawing";
        tileReady(QByteArray(), QGeoTiledMapReply::UnknownError, spec);
        return;
    }

    m_pixmap->save(&buffer, "png");
    buffer.close();
    tileReady(tileBuffer, QGeoTiledMapReply::NoError, spec);
    m_pixmap->fill(QColor(255,255,255));
}
