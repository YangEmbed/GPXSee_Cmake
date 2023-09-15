#include <QIODevice>
#include "gmifile.h"

static CalibrationPoint calibrationPoint(const QByteArray line)
{
	bool xOk, yOk, lonOk, latOk;
	QList<QByteArray> list = line.split(';');
	if (list.size() != 4)
		return CalibrationPoint();

	int x = list.at(0).toInt(&xOk);
	int y = list.at(1).toInt(&yOk);
	double lon = list.at(2).toDouble(&lonOk);
	double lat = list.at(3).toDouble(&latOk);

	return (xOk && yOk && latOk && lonOk)
	  ? CalibrationPoint(PointD(x, y), Coordinates(lon, lat))
	  : CalibrationPoint();
}

bool GmiFile::parse(QIODevice &device)
{
	int ln = 1;
	int width, height;
	bool ok;

	if (!device.open(QIODevice::ReadOnly)) {
		_errorString = device.errorString();
		return false;
	}

	while (!device.atEnd()) {
		QByteArray line = device.readLine(4096);

		if (ln == 1) {
			if (!line.startsWith("Map Calibration data file")) {
				_errorString = "Not a GMI file";
				return false;
			}
		} else if (ln == 2)
			_image = line.trimmed();
		else if (ln == 3) {
			width = line.toInt(&ok);
			if (!ok || ok <= 0) {
				_errorString = "Invalid image width";
				return false;
			}
		} else if (ln == 4) {
			height = line.toInt(&ok);
			if (!ok || ok <= 0) {
				_errorString = "Invalid image height";
				return false;
			}
			_size = QSize(width, height);
		} else {
			CalibrationPoint cp(calibrationPoint(line));
			if (cp.isValid())
				_points.append(cp);
			else
				break;
		}

		ln++;
	}

	device.close();

	return (_points.size() >= 2);
}

GmiFile::GmiFile(QIODevice &file)
{
	_valid = parse(file);
}