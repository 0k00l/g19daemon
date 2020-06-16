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

#include "hardwaremonitor.hpp"

//TODO add function to reload settings when settings file has been changed

HardwareMonitor::HardwareMonitor()
{
        Q_INIT_RESOURCE(hardwaremonitor);

        HwaSettings::getInstance()->loadSettings();
        screens = HwaSettings::getInstance()->getScreenList();

        if(screens.isEmpty())
        {
            screens.append(new StartScreen("StartSreen"));
            qDebug() << "Load StartScreen";
        }

        currentScreen_ = HwaSettings::getInstance()->getMainOrder()[0];
        currentMainScreen_ = currentScreen_;

        isActive = false;
        screen = new Gscreen(QImage(":/hardwaremonitor/icon.png"), tr("Hardware Monitor"));

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&HardwareMonitor::paint));
        timer->start(5000);
}

HardwareMonitor::~HardwareMonitor()
{
	delete screen;
}

QString HardwareMonitor::getName()
{
    return tr("Hardware Monitor");
}

void HardwareMonitor::lKeys(int keys)
{
    if (!isActive)
        return;

    QList<Screen*> mainOrder_ = HwaSettings::getInstance()->getMainOrder();
    QMap<QString, QList<Screen *>> subOrder_ = HwaSettings::getInstance()->getSubOrder();

    if (keys & G19Keys::G19_KEY_LRIGHT)
    {
            int position = mainOrder_.indexOf(currentMainScreen_) + 1;

            if (position >= mainOrder_.size())
            {
                    position = 0;
            }

            currentScreen_ = mainOrder_.at(position);
            currentMainScreen_ = mainOrder_.at(position);
    }

    else if (keys & G19Keys::G19_KEY_LLEFT)
    {
            int position = mainOrder_.indexOf(currentMainScreen_) - 1;

            if (position < 0)
            {
                    position = mainOrder_.size()-1;
            }

            currentScreen_ = mainOrder_.at(position);
            currentMainScreen_ = mainOrder_.at(position);
    }

    else if (keys & G19Keys::G19_KEY_LUP)
    {
            QList<Screen *> subScreen = subOrder_.value(currentMainScreen_->getName());

            int currentPosition = subScreen.indexOf(currentScreen_) - 1;

            if (currentPosition < 0)
            {
                    currentPosition = subScreen.size()-1;
            }

            currentScreen_ = subScreen.at(currentPosition);
    }

    else if (keys & G19Keys::G19_KEY_LDOWN)
    {
            QList<Screen *> subScreen = subOrder_.value(currentMainScreen_->getName());

            int currentPosition = subScreen.indexOf(currentScreen_) + 1;

            if (currentPosition >= subScreen.size())
            {
                    currentPosition = 0;
            }

            currentScreen_ = subScreen.at(currentPosition);
    }

    paint();
}

void HardwareMonitor::setActive(bool active)
{
	isActive = active;

	if (active)
	{
		paint();
	}
}

void HardwareMonitor::paint()
{
	if (!isActive)
		return;

        currentScreen_->draw(screen);

	emit doAction(displayFullScreen, screen);
}


bool HardwareMonitor::isPopup()
{
	return false;
}

QObject * HardwareMonitor::getQObject()
{
	return this;
}

QImage HardwareMonitor::getIcon()
{
        return QImage(":/hardwaremonitor/menu_icon.png");
}
