#include "line3D.h"

line3D::line3D(/* args */)
{

}

line3D::~line3D()
{

}

bool line3D::point3Dto2D(point_3d_t *source, point_2d_t *point)
{
    point->x = (source->x * cos_gamma) - (source->y * sin_gamma);
    point->y = -(source->x * sin_gamma * sin_alpha) - (source->y * cos_gamma * sin_alpha) + (source->z * cos_alpha);
    return true;
}

bool line3D::point2DToDisPoint(point_2d_t *point, uint8_t *x, uint8_t *y)
{

    *x = point->x + _zeroXoffset;
    *y = _zeroYOffset - point->y;
    return true;
}

bool line3D::printLine3D(TFT_eSprite *display, line_3d_t *line, uint32_t color)
{
    uint8_t start_x, start_y, end_x, end_y;
    point_2d_t point;
    point3Dto2D(&line->start_point, &point);
    point2DToDisPoint(&point, &start_x, &start_y);
    point3Dto2D(&line->end_point, &point);
    point2DToDisPoint(&point, &end_x, &end_y);

    display->drawLine(start_x, start_y, end_x, end_y, color);

    return true;
}

void line3D::RotatePoint(point_3d_t *point, double x, double y, double z)
{
    if (x != 0)
    {
        point->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
        point->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
        point->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}

void line3D::RotatePoint(point_3d_t *point, point_3d_t *point_new, double x, double y, double z)
{
    if (x != 0)
    {
        point_new->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point_new->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
        point_new->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point_new->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
        point_new->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point_new->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}