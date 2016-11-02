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

#include "qgeotilefetcherosmscout.h"
#include "qgeotiledmapreplyosmscoutraster.h"
#include "qosmscoutrenderer.h"
#include <QtLocation/private/qgeotilespec_p.h>
#include <QDebug>

QGeoTileFetcherOsmScout::QGeoTileFetcherOsmScout(QObject *parent) :
    QGeoTileFetcher(parent)
  ,m_renderer(new QOsmScoutRenderer)
  ,m_processing(false)
{
    qRegisterMetaType<QGeoTileSpec>();
    m_renderer->moveToThread(&m_rendererThread);
    connect(this, SIGNAL(render(QGeoTileSpec)), m_renderer, SLOT(render(QGeoTileSpec)), Qt::QueuedConnection);
    connect(m_renderer, SIGNAL(tileReady(QByteArray,int,QGeoTileSpec)), this, SLOT(processQueue()), Qt::QueuedConnection);
    m_rendererThread.start();
}

QGeoTileFetcherOsmScout::~QGeoTileFetcherOsmScout()
{
    m_renderer->deleteLater();
    m_rendererThread.quit();
    if(!m_rendererThread.wait())
    {
        m_rendererThread.terminate();
    }
}

QGeoTiledMapReply * QGeoTileFetcherOsmScout::getTileImage(const QGeoTileSpec &spec)
{
    if(!m_renderer)
    {
        qCritical() << "Tiles renderer is not initialized";
        Q_ASSERT(!m_renderer);
    }

    QGeoTiledMapReplyOsmScoutRaster* reply = 0;

    if(m_queue.indexOf(spec) >= 0)
    {
        m_queue.removeAll(spec);
        m_queue.enqueue(spec);
    }
    else
    {
        reply = new QGeoTiledMapReplyOsmScoutRaster(m_renderer, spec, this);
        connect(reply, SIGNAL(destroyed(QObject*)), this, SLOT(replyRemoved(QObject*)));
        m_replies.insert(spec, reply);
        m_queue.enqueue(spec);
    }

    if(!m_processing)
    {
        processQueue();
    }
    return reply;
}

void QGeoTileFetcherOsmScout::processQueue()
{
    m_processing = false;
    if(m_queue.isEmpty())
    {
        return;
    }

    QGeoTileSpec spec = m_queue.takeFirst();
    QGeoTiledMapReplyOsmScoutRaster* reply = m_replies.value(spec, 0);
    if(reply)
    {
        m_processing = true;
        connect(m_renderer, SIGNAL(tileReady(QByteArray,int,QGeoTileSpec)), reply, SLOT(onTileReady(QByteArray,int,QGeoTileSpec)), Qt::QueuedConnection);
        emit render(spec);
    }
    m_replies.remove(spec);
}

void QGeoTileFetcherOsmScout::replyRemoved(QObject* obj)
{
    QGeoTiledMapReplyOsmScoutRaster* reply = static_cast<QGeoTiledMapReplyOsmScoutRaster*>(obj);
    QGeoTileSpec spec = m_replies.key(reply);
    m_queue.removeAll(spec);
    m_replies.remove(spec);
}
