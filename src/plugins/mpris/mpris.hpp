/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MPRIS_H
#define MPRIS_H

#include <QtCore>
#include <QtPlugin>
#include <QtConcurrent/QtConcurrent>
#include <QtDBus/QtDBus>
#include <QTimer>
#include <QFont>
#include <QFontMetrics>
#include <QSettings>
#include "../../plugininterface.hpp"
#include "../../gscreen.hpp"
#include "mprisfetcher2.hpp"

class Mpris : public QObject, PluginInterface
{
		Q_OBJECT
		Q_INTERFACES(PluginInterface)

		Q_PLUGIN_METADATA(IID "your-string-here" FILE "")

	public:
		Mpris();
		~Mpris();
		void LKeys(int keys);
		QString getName();
		QImage getIcon();
		void setActive(bool active);
		bool isPopup();
		QObject *getQObject();

	private:
		gScreen *screen;
		bool isActive;
		QStringList players; 
		QString player;
		MprisFetcher2 *mpris;
		MediaData *mediadata;
		PlayerStatus *playerstatus;
		QTimer *timer;
		int LastPos[4];
		int LastPosDir[4];
		int speed;
		bool menuActive;
		int menuSelect;
		QSettings *settings;
		
		QStringList getPlayersList();
		void paint();
		void menu();

	public slots:
		void onStatusChanged(PlayerStatus);
		void onTrackChanged(MediaData);
		void onTimer();

	signals:
		void doAction(gAction action, void *data);			// Signal to draw img on screen
};

#endif // MPRIS_H
