#ifndef HASH_H
#define HASH_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QPoint>
#include <QPair>
#include <QHash>

#define HASH_T uint

inline uint qHash(const QPoint &p)
{
	return qHash(QPair<int, int>(p.x(), p.y()));
}
#else // QT6
#define HASH_T size_t
#endif // QT6

#endif // HASH_H
