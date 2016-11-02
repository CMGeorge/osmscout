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

#ifndef QOSMSCOUTRENDERER_H
#define QOSMSCOUTRENDERER_H

#include <QObject>
#include <QtCore/QScopedPointer>
#include <osmscout/Database.h>
#include <osmscout/MapService.h>
#include <osmscout/MapPainter.h>
#include <osmscout/StyleConfigLoader.h>

#include "qgeotiledmapreplyosmscoutraster.h"

QT_BEGIN_NAMESPACE

class QOsmScoutRendererPrivate : public QObject
{
public:
    QOsmScoutRendererPrivate();
    ~QOsmScoutRendererPrivate(){}//TODO: implement destructor
    osmscout::DatabaseParameter databaseParameter;
    osmscout::DatabaseRef       database;
    osmscout::MapServiceRef     mapService;
    osmscout::MapParameter        mapParameter;
    osmscout::MapData             mapData;
    osmscout::MapPainter*    mapPainter;
    osmscout::AreaSearchParameter searchParameter;
    osmscout::StyleConfig *styleConfig;
    QString stylePath;
    QString mapPath;
    bool databaseOpen;
};

class QGeoTiledMapReplyOsmScoutRaster;
class QPainter;

class QOsmScoutRenderer : public QObject
{
    Q_OBJECT
public:
    explicit QOsmScoutRenderer(QObject *parent = 0);
    ~QOsmScoutRenderer();
signals:
    void tileReady(const QByteArray& tileData, int error, const QGeoTileSpec& spec);

public slots:
    void render(const QGeoTileSpec& spec);

private:
    QPixmap* m_pixmap;
    QPainter* m_qtPainter;
    QScopedPointer<QOsmScoutRendererPrivate> d;
};
QT_END_NAMESPACE

#endif // QOSMSCOUTRENDERER_H
