
#ifndef DIVELIGHT_COLUMNRENDERER_HPP
#define DIVELIGHT_COLUMNRENDERER_HPP

#include "renderer.hpp"

class ColumnRenderer : public IRenderer
{
public:
    ColumnRenderer(unsigned int columnCount);
    virtual void setData(float *data);
private:
    unsigned int m_columnCount;
};


#endif //DIVELIGHT_COLUMNRENDERER_HPP
