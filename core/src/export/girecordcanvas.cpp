// Copyright (c) 2013, https://github.com/rhcad/touchvg

#include "girecordshape.h"
#include "girecordcanvas.h"
#include "mgshapes.h"
#include "mgshapet.h"
#include "mgstorage.h"
#include <string>

struct CmdSetPen : public MgRecordShape::ICmd {
    int argb; float width; int style; float phase; float orgw;
    CmdSetPen() : argb(0xFF000000), width(0), style(0), phase(0), orgw(0) {}
    CmdSetPen(int argb, float width, int style, float phase, float orgw)
        : argb(argb), width(width), style(style), phase(), orgw(orgw) {}
    
    virtual int type() const { return 1; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdSetPen& p = (const CmdSetPen&)src;
            argb = p.argb;
            width = p.width;
            style = p.style;
            phase = p.phase;
            orgw = p.orgw;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeInt("argb", argb);
        s->writeFloat("width", width);
        s->writeInt("style", style);
        s->writeFloat("phase", phase);
        s->writeFloat("orgw", orgw);
        return true;
    }
    virtual bool load(MgStorage* s) {
        argb = s->readInt("argb", argb);
        width = s->readFloat("width", width);
        style = s->readInt("style", style);
        phase = s->readFloat("phase", phase);
        orgw = s->readFloat("orgw", orgw);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d&) const {
        gs.getCanvas()->setPen(argb, width, style, phase, orgw);
    }
};

struct CmdSetBrush : public MgRecordShape::ICmd {
    int argb; int style;
    CmdSetBrush() {}
    CmdSetBrush(int argb, int style) : argb(argb), style(style) {}
    
    virtual int type() const { return 2; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdSetBrush& p = (const CmdSetBrush&)src;
            argb = p.argb;
            style = p.style;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeInt("argb", argb);
        s->writeInt("style", style);
        return true;
    }
    virtual bool load(MgStorage* s) {
        argb = s->readInt("argb", argb);
        style = s->readInt("style", style);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d&) const {
        gs.getCanvas()->setBrush(argb, style);
    }
};

struct CmdClearRect : public MgRecordShape::ICmd {
    Point2d pt; Vector2d vec;
    CmdClearRect() {}
    CmdClearRect(const Matrix2d& d2w, float x, float y, float w, float h)
        : pt(Point2d(x, y) * d2w), vec(Vector2d(w, h) * d2w) {}
    
    virtual int type() const { return 3; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdClearRect& p = (const CmdClearRect&)src;
            pt = p.pt;
            vec = p.vec;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        s->writeFloat("w", vec.x);
        s->writeFloat("h", vec.y);
        return true;
    }
    virtual bool load(MgStorage* s) {
        pt.set(s->readFloat("x", pt.x), s->readFloat("y", pt.y));
        vec.set(s->readFloat("w", vec.x), s->readFloat("h", vec.y));
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt2(pt * w2d);
        Vector2d vec2(vec * w2d);
        gs.getCanvas()->clearRect(pt2.x, pt2.y, vec2.x, vec2.y);
    }
    virtual Box2d getExtent() const { return Box2d(pt, pt + vec); }
};

struct CmdDrawRect : public MgRecordShape::ICmd {
    Point2d pt; Vector2d vec; bool stroke; bool fill;
    CmdDrawRect() {}
    CmdDrawRect(const Matrix2d& d2w, float x, float y, float w, float h, bool stroke, bool fill)
        : pt(Point2d(x, y) * d2w), vec(Vector2d(w, h) * d2w), stroke(stroke), fill(fill) {}
    
    virtual int type() const { return 4; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdDrawRect& p = (const CmdDrawRect&)src;
            pt = p.pt;
            vec = p.vec;
            stroke = p.stroke;
            fill = p.fill;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        s->writeFloat("w", vec.x);
        s->writeFloat("h", vec.y);
        s->writeBool("stroke", stroke);
        s->writeBool("fill", fill);
        return true;
    }
    virtual bool load(MgStorage* s) {
        pt.set(s->readFloat("x", pt.x), s->readFloat("y", pt.y));
        vec.set(s->readFloat("w", vec.x), s->readFloat("h", vec.y));
        stroke = s->readBool("stroke", stroke);
        fill = s->readBool("fill", fill);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt2(pt * w2d);
        Vector2d vec2(vec * w2d);
        gs.getCanvas()->drawRect(pt2.x, pt2.y, vec2.x, vec2.y, stroke, fill);
    }
    virtual Box2d getExtent() const { return Box2d(pt, pt + vec); }
};

struct CmdDrawLine : public MgRecordShape::ICmd {
    Point2d pt1; Point2d pt2;
    CmdDrawLine() {}
    CmdDrawLine(const Matrix2d& d2w, float x1, float y1, float x2, float y2)
        : pt1(Point2d(x1, y1) * d2w), pt2(Point2d(x2, y2) * d2w) {}
    
    virtual int type() const { return 5; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdDrawLine& p = (const CmdDrawLine&)src;
            pt1 = p.pt1;
            pt2 = p.pt2;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("x1", pt1.x);
        s->writeFloat("y1", pt1.y);
        s->writeFloat("x2", pt2.x);
        s->writeFloat("y2", pt2.y);
        return true;
    }
    virtual bool load(MgStorage* s) {
        pt1.set(s->readFloat("x1", pt1.x), s->readFloat("y1", pt1.y));
        pt2.set(s->readFloat("x2", pt2.x), s->readFloat("y2", pt2.y));
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt3(pt1 * w2d);
        Point2d pt4(pt2 * w2d);
        gs.getCanvas()->drawLine(pt3.x, pt3.y, pt4.x, pt4.y);
    }
    virtual Box2d getExtent() const { return Box2d(pt1, pt2); }
};

struct CmdDrawEllipse : public MgRecordShape::ICmd {
    Point2d pt; Vector2d vec; bool stroke; bool fill;
    CmdDrawEllipse() {}
    CmdDrawEllipse(const Matrix2d& d2w, float x, float y, float w, float h, bool stroke, bool fill)
        : pt(Point2d(x, y) * d2w), vec(Vector2d(w, h) * d2w), stroke(stroke), fill(fill) {}
    
    virtual int type() const { return 6; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdDrawEllipse& p = (const CmdDrawEllipse&)src;
            pt = p.pt;
            vec = p.vec;
            stroke = p.stroke;
            fill = p.fill;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        s->writeFloat("w", vec.x);
        s->writeFloat("h", vec.y);
        s->writeBool("stroke", stroke);
        s->writeBool("fill", fill);
        return true;
    }
    virtual bool load(MgStorage* s) {
        pt.set(s->readFloat("x", pt.x), s->readFloat("y", pt.y));
        vec.set(s->readFloat("w", vec.x), s->readFloat("h", vec.y));
        stroke = s->readBool("stroke", stroke);
        fill = s->readBool("fill", fill);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt2(pt * w2d);
        Vector2d vec2(vec * w2d);
        gs.getCanvas()->drawEllipse(pt2.x, pt2.y, vec2.x, vec2.y, stroke, fill);
    }
    virtual Box2d getExtent() const { return Box2d(pt, pt + vec); }
};

struct CmdBeginPath : public MgRecordShape::ICmd {
    CmdBeginPath() {}
    
    virtual int type() const { return 7; }
    virtual void copy(const ICmd&) {}
    virtual bool save(MgStorage*) const {
        return true;
    }
    virtual bool load(MgStorage*) {
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d&) const {
        gs.getCanvas()->beginPath();
    }
};

struct CmdMoveTo : public MgRecordShape::ICmd {
    Point2d pt;
    CmdMoveTo() {}
    CmdMoveTo(const Matrix2d& d2w, float x, float y) : pt(Point2d(x, y) * d2w) {}
    
    virtual int type() const { return 8; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdMoveTo& p = (const CmdMoveTo&)src;
            pt = p.pt;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        return true;
    }
    virtual bool load(MgStorage* s) {
        pt.set(s->readFloat("x", pt.x), s->readFloat("y", pt.y));
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt2(pt * w2d);
        gs.getCanvas()->moveTo(pt2.x, pt2.y);
    }
    virtual Box2d getExtent() const { return Box2d(pt, 1e-3f, 0); }
};

struct CmdLineTo : public MgRecordShape::ICmd {
    Point2d pt;
    CmdLineTo() {}
    CmdLineTo(const Matrix2d& d2w, float x, float y) : pt(Point2d(x, y) * d2w) {}
    
    virtual int type() const { return 9; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdLineTo& p = (const CmdLineTo&)src;
            pt = p.pt;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        return true;
    }
    virtual bool load(MgStorage* s) {
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt2(pt * w2d);
        gs.getCanvas()->lineTo(pt2.x, pt2.y);
    }
    virtual Box2d getExtent() const { return Box2d(pt, 1e-3f, 0); }
};

struct CmdBezierTo : public MgRecordShape::ICmd {
    Point2d c1, c2, pt;
    CmdBezierTo() {}
    CmdBezierTo(const Matrix2d& d2w, float c1x, float c1y, float c2x, float c2y, float x, float y)
        : c1(Point2d(c1x, c1y) * d2w), c2(Point2d(c2x, c2y) * d2w), pt(Point2d(x, y) * d2w) {}
    
    virtual int type() const { return 10; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdBezierTo& p = (const CmdBezierTo&)src;
            c1 = p.c1;
            c2 = p.c2;
            pt = p.pt;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("c1x", c1.x);
        s->writeFloat("c1y", c1.y);
        s->writeFloat("c2x", c2.x);
        s->writeFloat("c2y", c2.y);
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        return true;
    }
    virtual bool load(MgStorage* s) {
        c1.x = s->readFloat("c1x", c1.x);
        c1.y = s->readFloat("c1y", c1.y);
        c2.x = s->readFloat("c2x", c2.x);
        c2.y = s->readFloat("c2y", c2.y);
        pt.x = s->readFloat("x", pt.x);
        pt.y = s->readFloat("y", pt.y);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d c1t(c1 * w2d), c2t(c2 * w2d), pt2(pt * w2d);
        gs.getCanvas()->bezierTo(c1t.x, c1t.y, c2t.x, c2t.y, pt2.x, pt2.y);
    }
    virtual Box2d getExtent() const { return Box2d(pt, c1, c2, pt); }
};

struct CmdQuadTo : public MgRecordShape::ICmd {
    Point2d cp, pt;
    CmdQuadTo() {}
    CmdQuadTo(const Matrix2d& d2w, float cpx, float cpy, float x, float y)
        : cp(Point2d(cpx, cpy) * d2w), pt(Point2d(x, y) * d2w) {}
    
    virtual int type() const { return 11; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdQuadTo& p = (const CmdQuadTo&)src;
            cp = p.cp;
            pt = p.pt;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("cpx", cp.x);
        s->writeFloat("cpy", cp.y);
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        return true;
    }
    virtual bool load(MgStorage* s) {
        cp.x = s->readFloat("cpx", cp.x);
        cp.y = s->readFloat("cpy", cp.y);
        pt.x = s->readFloat("x", pt.x);
        pt.y = s->readFloat("y", pt.y);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d cp2(cp * w2d), pt2(pt * w2d);
        gs.getCanvas()->quadTo(cp2.x, cp2.y, pt2.x, pt2.y);
    }
    virtual Box2d getExtent() const { return Box2d(cp, pt); }
};

struct CmdClosePath : public MgRecordShape::ICmd {
    CmdClosePath() {}
    
    virtual int type() const { return 12; }
    virtual void copy(const ICmd&) {}
    virtual bool save(MgStorage*) const {
        return true;
    }
    virtual bool load(MgStorage*) {
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d&) const {
        gs.getCanvas()->closePath();
    }
};

struct CmdDrawPath : public MgRecordShape::ICmd {
    bool stroke; bool fill;
    CmdDrawPath() {}
    CmdDrawPath(bool stroke, bool fill) : stroke(stroke), fill(fill) {}
    
    virtual int type() const { return 13; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdDrawPath& p = (const CmdDrawPath&)src;
            stroke = p.stroke;
            fill = p.fill;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeBool("stroke", stroke);
        s->writeBool("fill", fill);
        return true;
    }
    virtual bool load(MgStorage* s) {
        stroke = s->readBool("stroke", stroke);
        fill = s->readBool("fill", fill);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d&) const {
        gs.getCanvas()->drawPath(stroke, fill);
    }
};

struct CmdDrawHandle : public MgRecordShape::ICmd {
    Point2d pt; int t;
    CmdDrawHandle() {}
    CmdDrawHandle(const Matrix2d& d2w, float x, float y, int t) : pt(Point2d(x, y) * d2w), t(t) {}
    
    virtual int type() const { return 14; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdDrawHandle& p = (const CmdDrawHandle&)src;
            pt = p.pt;
            t = p.t;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        s->writeInt("t", t);
        return true;
    }
    virtual bool load(MgStorage* s) {
        pt.x = s->readFloat("x", pt.x);
        pt.y = s->readFloat("y", pt.y);
        t = s->readInt("t", t);
        return true;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt2(pt * w2d);
        gs.getCanvas()->drawHandle(pt2.x, pt2.y, t);
    }
    virtual Box2d getExtent() const { return Box2d(pt, 1e-3f, 0); }
};

struct CmdDrawBitmap : public MgRecordShape::ICmd {
    std::string name; Point2d pt; Vector2d vec; float angle;
    CmdDrawBitmap() {}
    CmdDrawBitmap(const Matrix2d& d2w, const char* name, float xc, float yc, float w, float h, float angle)
        : name(name), pt(Point2d(xc, yc) * d2w), vec(Vector2d(w, h) * d2w), angle(angle) {}
    
    virtual int type() const { return 15; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdDrawBitmap& p = (const CmdDrawBitmap&)src;
            name = p.name;
            pt = p.pt;
            vec = p.vec;
            angle = p.angle;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeString("name", name.c_str());
        s->writeFloat("xc", pt.x);
        s->writeFloat("yc", pt.y);
        s->writeFloat("w", vec.x);
        s->writeFloat("h", vec.y);
        s->writeFloat("angle", angle);
        return true;
    }
    virtual bool load(MgStorage* s) {
        int len = s->readString("name", NULL, 0);
        name.resize(len, 0);
        len = s->readString("name", const_cast<char*>(name.c_str()), (int)name.size());
        
        pt.x = s->readFloat("xc", pt.x);
        pt.y = s->readFloat("yc", pt.y);
        vec.x = s->readFloat("w", vec.x);
        vec.y = s->readFloat("h", vec.y);
        angle = s->readFloat("angle", angle);
        
        return len > 0;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt2(pt * w2d);
        Vector2d vec2(vec * w2d);
        gs.getCanvas()->drawBitmap(name.c_str(), pt2.x, pt2.y, vec2.x, vec2.y, angle);
    }
    virtual Box2d getExtent() const { return Box2d(pt, pt + vec); }
};

struct CmdDrawTextAt : public MgRecordShape::ICmd {
    std::string text; Point2d pt; Vector2d vec; int align;
    CmdDrawTextAt() {}
    CmdDrawTextAt(const Matrix2d& d2w, const char* text, float x, float y, float h, int align)
        : text(text), pt(Point2d(x, y) * d2w), vec(Vector2d(h, h) * d2w), align(align) {}
    
    virtual int type() const { return 16; }
    virtual void copy(const ICmd& src) {
        if (src.type() == type()) {
            const CmdDrawTextAt& p = (const CmdDrawTextAt&)src;
            text = p.text;
            pt = p.pt;
            vec = p.vec;
            align = p.align;
        }
    }
    virtual bool save(MgStorage* s) const {
        s->writeString("text", text.c_str());
        s->writeFloat("x", pt.x);
        s->writeFloat("y", pt.y);
        s->writeFloat("h", vec.x);
        s->writeInt("align", align);
        return true;
    }
    virtual bool load(MgStorage* s) {
        int len = s->readString("text", NULL, 0);
        text.resize(len, 0);
        len = s->readString("text", const_cast<char*>(text.c_str()), (int)text.size());
        
        pt.x = s->readFloat("x", pt.x);
        pt.y = s->readFloat("y", pt.y);
        vec.x = s->readFloat("h", vec.x);
        align = s->readInt("align", align);
        
        return len > 0;
    }
    virtual void draw(GiGraphics& gs, const Matrix2d& w2d) const {
        Point2d pt2(pt * w2d);
        Vector2d vec2(vec * w2d);
        gs.getCanvas()->drawTextAt(text.c_str(), pt2.x, pt2.y, vec2.x, align);
    }
    virtual Box2d getExtent() const { return Box2d(pt, pt + vec); }
};

// MgRecordShape
//

MgRecordShape::ICmd* MgRecordShape::createItem(int type) const
{
    switch (type) {
        case 1: return new CmdSetPen();
        case 2: return new CmdSetBrush();
        case 3: return new CmdClearRect();
        case 4: return new CmdDrawRect();
        case 5: return new CmdDrawLine();
        case 6: return new CmdDrawEllipse();
        case 7: return new CmdBeginPath();
        case 8: return new CmdMoveTo();
        case 9: return new CmdLineTo();
        case 10: return new CmdBezierTo();
        case 11: return new CmdQuadTo();
        case 12: return new CmdClosePath();
        case 13: return new CmdDrawPath();
        case 14: return new CmdDrawHandle();
        case 15: return new CmdDrawBitmap();
        case 16: return new CmdDrawTextAt();
    }
    return NULL;
}

void MgRecordShape::_clear()
{
    for (ITEMS::iterator it = _items.begin(); it != _items.end(); ++it) {
        delete *it;
    }
    _items.clear();
    _sid = 0;
}

MgObject* MgRecordShape::clone() const
{
    MgRecordShape* p = new MgRecordShape();
    p->copy(*this);
    return p;
}

void MgRecordShape::copy(const MgObject& src)
{
    if (src.isKindOf(Type()) && this != &src) {
        const MgRecordShape& p = (const MgRecordShape&)src;
        _clear();
        for (ITEMS::const_iterator it = p._items.begin(); it != p._items.end(); ++it) {
            ICmd* item = createItem((*it)->type());
            item->copy(**it);
            _items.push_back(item);
        }
        _sid = p._sid;
    }
    MgBaseShape::copy(src);
}

bool MgRecordShape::equals(const MgObject& src) const
{
    if (src.isKindOf(Type())) {
        const MgRecordShape& p = (const MgRecordShape&)src;
        if (_items.size() != p._items.size() || _sid != p._sid)
            return false;
    }
    return MgBaseShape::equals(src);
}

bool MgRecordShape::save(MgStorage* s) const
{
    int i = 0;
    bool ret = true;
    
    s->writeInt("refid", _sid);
    for (ITEMS::const_iterator it = _items.begin(); ret && it != _items.end(); ++it) {
        ret = s->writeNode("p", i, false);
        if (ret) {
            s->writeInt("type", (*it)->type());
            ret = (*it)->save(s);
            s->writeNode("p", i++, true);
        }
    }
    return ret && _save(s);
}

bool MgRecordShape::load(MgShapeFactory* factory, MgStorage* s)
{
    _clear();
    
    _sid = s->readInt("refid", _sid);
    for (int i = 0; s->readNode("p", i, false); i++) {
        ICmd* item = createItem(s->readInt("type", 0));
        
        if (item && item->load(s)) {
            _items.push_back(item);
        } else {
            delete item;
        }
        s->readNode("p", i, true);
    }
    return _load(factory, s);
}

bool MgRecordShape::draw(int mode, GiGraphics& gs, const GiContext& ctx, int segment) const
{
    const Matrix2d& w2d = gs.xf().worldToDisplay();
    for (ITEMS::const_iterator it = _items.begin(); it != _items.end(); ++it) {
        (*it)->draw(gs, w2d);
    }
    return _draw(mode, gs, ctx, segment) || !_items.empty();
}

void MgRecordShape::addItem(ICmd* p)
{
    _extent.unionWith(p->getExtent());
    _items.push_back(p);
}

// GiRecordCanvas
//

GiRecordCanvas::GiRecordCanvas(MgShapes* shapes, const GiTransform* xf, int ignoreId)
    : _shapes(shapes), _xf(xf), _ignoreId(ignoreId)
{
    _shape = MgShapeT<MgRecordShape>::create();
    _sp = (MgRecordShape*)_shape->shape();
}

const Matrix2d GiRecordCanvas::d2w() const
{
    return _xf->displayToWorld();
}

void GiRecordCanvas::clear()
{
    if (_shape) {
        if (_sp->getCount() > 0) {
            _shapes->addShapeDirect(_shape);
        } else {
            _shape->release();
        }
        _shape = NULL;
        _sp = NULL;
    }
}

bool GiRecordCanvas::beginShape(int, int sid, int, float, float, float, float)
{
    if (sid == _ignoreId) {
        return false;
    }
    if (!_shape || _sp->getCount() > 0) {
        clear();
        _shape = MgShapeT<MgRecordShape>::create();
        _sp = (MgRecordShape*)_shape->shape();
    }
    _sp->setRefID(sid);
    
    return true;
}

void GiRecordCanvas::endShape(int, int, float, float)
{
    clear();
    _shape = MgShapeT<MgRecordShape>::create();
    _sp = (MgRecordShape*)_shape->shape();
}

void GiRecordCanvas::setPen(int argb, float width, int style, float phase, float orgw)
{
    _sp->addItem(new CmdSetPen(argb, width, style, phase, orgw));
}

void GiRecordCanvas::setBrush(int argb, int style)
{
    _sp->addItem(new CmdSetBrush(argb, style));
}

void GiRecordCanvas::clearRect(float x, float y, float w, float h)
{
    _sp->addItem(new CmdClearRect(d2w(), x, y, w, h));
}

void GiRecordCanvas::drawRect(float x, float y, float w, float h, bool stroke, bool fill)
{
    _sp->addItem(new CmdDrawRect(d2w(), x, y, w, h, stroke, fill));
}

void GiRecordCanvas::drawLine(float x1, float y1, float x2, float y2)
{
    _sp->addItem(new CmdDrawLine(d2w(), x1, y1, x2, y2));
}

void GiRecordCanvas::drawEllipse(float x, float y, float w, float h, bool stroke, bool fill)
{
    _sp->addItem(new CmdDrawEllipse(d2w(), x, y, w, h, stroke, fill));
}

void GiRecordCanvas::beginPath()
{
    _sp->addItem(new CmdBeginPath());
}

void GiRecordCanvas::moveTo(float x, float y)
{
    _sp->addItem(new CmdMoveTo(d2w(), x, y));
}

void GiRecordCanvas::lineTo(float x, float y)
{
    _sp->addItem(new CmdLineTo(d2w(), x, y));
}

void GiRecordCanvas::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y)
{
    _sp->addItem(new CmdBezierTo(d2w(), c1x, c1y, c2x, c2y, x, y));
}

void GiRecordCanvas::quadTo(float cpx, float cpy, float x, float y)
{
    _sp->addItem(new CmdQuadTo(d2w(), cpx, cpy, x, y));
}

void GiRecordCanvas::closePath()
{
    _sp->addItem(new CmdClosePath());
}

void GiRecordCanvas::drawPath(bool stroke, bool fill)
{
    _sp->addItem(new CmdDrawPath(stroke, fill));
}

void GiRecordCanvas::saveClip()
{
}

void GiRecordCanvas::restoreClip()
{
}

bool GiRecordCanvas::clipRect(float, float, float, float)
{
    return false;
}

bool GiRecordCanvas::clipPath()
{
    return false;
}

bool GiRecordCanvas::drawHandle(float x, float y, int type)
{
    _sp->addItem(new CmdDrawHandle(d2w(), x, y, type));
    return true;
}

bool GiRecordCanvas::drawBitmap(const char* name, float xc, float yc,
                                float w, float h, float angle)
{
    _sp->addItem(new CmdDrawBitmap(d2w(), name, xc, yc, w, h, angle));
    return true;
}

float GiRecordCanvas::drawTextAt(const char* text, float x, float y, float h, int align)
{
    _sp->addItem(new CmdDrawTextAt(d2w(), text, x, y, h, align));
    return h;
}
