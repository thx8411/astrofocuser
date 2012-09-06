/*
 * copyright (c) 2012 Blaise-Florentin Collin
 *
 * This file is part of astrofocuser.
 *
 * astrofocuser is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v3
 * as published by the Free Software Foundation
 *
 * astrofocuser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * v3 along with astrofocuser; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include "qfwhm.hpp"

QFwhm::QFwhm()
{
   // fwhm default value
   fwhm_value=0.0;

   // main window
   setWindowTitle(QString::fromUtf8("FWHM"));
   setAttribute(Qt::WA_TranslucentBackground);
   setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   setFixedSize(FWHM_WIDTH,FWHM_HEIGHT+FWHM_LABEL_HEIGHT);

   // picture hole for grabbing
   hole=new QWidget();
   hole->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   hole->setFixedSize(FWHM_WIDTH,FWHM_HEIGHT);

   // FWHM value
   value=new QLabel();
   value->setStyleSheet("QLabel { background-color : black; color : yellow; font-size: 22px;}");
   value->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   value->setFixedSize(FWHM_WIDTH,FWHM_LABEL_HEIGHT);
   value->setAlignment(Qt::AlignRight);

   // setting label text
   fwhm_text.setNum(fwhm_value,'f',2);
   value->setText(fwhm_text);

   // Widget placement
   grid=new QVBoxLayout();
   grid->addWidget(hole,Qt::AlignTop);
   grid->addWidget(value,Qt::AlignBottom);
   grid->setContentsMargins (0,0,0,0);
   setLayout(grid);

   // display widget
   show();

   // timer loop
   timer=new QTimer();
   connect(timer, SIGNAL(timeout()), this, SLOT(compute()));
   timer->start(100);
}

QFwhm::~QFwhm() {
   delete hole;
   delete value;
   delete grid;
   delete timer;
}

void QFwhm::compute() {
   // vars
   int i,j,n;
   double mean=0;
   double x2_sum=0;
   int pixel_value;
   QRgb plot;

   //  grab the picture
   QRect position;
   position=geometry();
   background=QPixmap::grabWindow(QApplication::desktop()->winId(),position.x(),position.y(),FWHM_WIDTH,FWHM_HEIGHT);
   image=background.toImage();

   // compute the fwhm
   n=FWHM_WIDTH*FWHM_HEIGHT;
   // mean
   for(i=0;i<FWHM_WIDTH;i++) {
      for(j=0;j<FWHM_HEIGHT;j++) {
         plot=image.pixel(i,j);
         pixel_value=qGray(plot);
         mean+=(double)pixel_value;
      }
   }
   // pow 2
   mean/=(double)n;
   for(int i=0;i<FWHM_WIDTH;i++) {
      for(int j=0;j<FWHM_HEIGHT;j++) {
         plot=image.pixel(i,j);
         pixel_value=qGray(plot);
         x2_sum+=pow((double)pixel_value-mean,2);
      }
   }
   x2_sum/=(double)n;
   // final value
   fwhm_value=sqrt(x2_sum)*2.3548;

   // update the label value
   fwhm_text.setNum(fwhm_value,'f',2);
   value->setText(fwhm_text);
}
