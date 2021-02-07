#ifndef __LINE3D_H__
#define __LINE3D_H__

#include <Arduino.h>
#include "utility/Sprite.h"

typedef struct
{
    double x;
    double y;
    double z;
} point_3d_t;

typedef struct
{
    point_3d_t start_point;
    point_3d_t end_point;
} line_3d_t;

typedef struct
{
    double x;
    double y;
} point_2d_t;

class line3D
{
private:

    double r_rand = PI / 180;

    double r_alpha = 19.47 * PI / 180;
    double r_gamma = 20.7 * PI / 180;

    double sin_alpha = sin(19.47 * PI / 180);
    double cos_alpha = cos(19.47 * PI / 180);
    double sin_gamma = sin(20.7 * PI / 180);
    double cos_gamma = cos(20.7 * PI / 180);

    int _zeroXoffset = 0;
    int _zeroYOffset = 0;

public:
    line3D(/* args */);
    ~line3D();

    void setZeroOffset(int offsetX,int offsetY){ _zeroXoffset = offsetX;  _zeroYOffset = offsetY;}
    bool point3Dto2D(point_3d_t *source, point_2d_t *point);
    bool point2DToDisPoint(point_2d_t *point, uint8_t *x, uint8_t *y);
    bool printLine3D(TFT_eSprite *display, line_3d_t *line, uint32_t color);
    void RotatePoint(point_3d_t *point, double x, double y, double z);
    void RotatePoint(point_3d_t *point, point_3d_t *point_new, double x, double y, double z);

};


#endif