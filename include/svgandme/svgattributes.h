#pragma once

#include <memory>
#include <vector>
#include <cstdint>		// uint8_t, etc
#include <cstddef>		// nullptr_t, ptrdiff_t, size_t


#include "maths.h"
#include "xmlscan.h"

#include "svgstructuretypes.h"


namespace waavs {
    //
    // SVGPatternExtendMode
    // 
	// A structure that represents the extend mode of a pattern.
	struct SVGPatternExtendMode : public SVGVisualProperty 
    {
        static void registerFactory() {
            registerSVGAttribute("extendMode", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGPatternExtendMode>(nullptr);
                node->loadFromAttributes(attrs);
                return node;
                });
        }
        
        BLExtendMode fExtendMode{ BL_EXTEND_MODE_REPEAT };      // repeat by default

        SVGPatternExtendMode(IAmGroot* iMap) : SVGVisualProperty(iMap) 
        {
            id("extendMode");
            autoDraw(false);
        }

        BLExtendMode value() const { return fExtendMode; }
        
        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {   
            bool success = getEnumValue(SVGExtendMode, inChunk, (int&)fExtendMode);
            set(success);

            return success;
        }
        

	};
}




// Specific types of attributes
namespace waavs {

    //
    // SVGOpacity
    // https://svgwg.org/svg2-draft/render.html#ObjectAndGroupOpacityProperties
    // Opacity, when applied to a group, should create a backing
    // store to be created.
    // We don't need to set the global opacity as part of drawing
    // this attribute.  Allow the specific geometry to do what it will 
    // with this attribute during it's own drawing.  That might include
    // simply inheriting the value.
    //
    struct SVGOpacity : public SVGVisualProperty
    {
        static void registerFactory() {
			registerSVGAttribute("opacity", [](const XmlAttributeCollection& attrs) {
				auto node = std::make_shared<SVGOpacity>(nullptr);
			node->loadFromAttributes(attrs);
			return node;
				});
        }
        
        double fValue{1};
        BLVar fOpacityVar;
        
        SVGOpacity(IAmGroot* groot) :SVGVisualProperty(groot) { id("opacity"); }

        const BLVar getVariant() noexcept override { return fOpacityVar; }
        
        
        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
			ctx->globalOpacity(fValue);
        }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            if (!inChunk)
                return false;
            
            SVGDimension dim;
			dim.loadFromChunk(inChunk);
			fValue = dim.calculatePixels(1.0);
            fOpacityVar = fValue;
            
            set(true);
			needsBinding(false);
            
            return true;
        }
        

    };
    
    struct SVGFillOpacity : public SVGOpacity
    {
        static void registerFactory() {
            registerSVGAttribute("fill-opacity", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGFillOpacity>(nullptr); 
                node->loadFromAttributes(attrs);
                return node;
                });
            
        }
        
        SVGFillOpacity(IAmGroot* iMap)  :SVGOpacity(iMap)  { id("fill-opacity");}
        
        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
			ctx->fillOpacity(fValue);
        }

    };

    struct SVGStrokeOpacity : public SVGOpacity
    {
        static void registerFactory() {
            registerSVGAttribute("stroke-opacity", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGStrokeOpacity>(nullptr); 
                node->loadFromAttributes(attrs);
                return node; 
                });

        }

        SVGStrokeOpacity(IAmGroot* iMap) :SVGOpacity(iMap) { id("stroke-opacity"); }

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->strokeOpacity(fValue);
        }

    };

}

namespace waavs {
    struct SVGPaintOrderAttribute : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("paint-order", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGPaintOrderAttribute>(nullptr); 
                node->loadFromAttributes(attrs);
                return node; 
                });
        }

        ByteSpan fValue{};
        
        
        SVGPaintOrderAttribute(IAmGroot* iMap) :SVGVisualProperty(iMap) 
        {
            id("paint-order");
        }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            if (!inChunk)
                return false;
            
            fValue = inChunk;
            
            set(true);

            return true;
        }
        
    };


}

//==================================================================
//  SVG Text Properties
//==================================================================
// Typography Attributes
namespace waavs {
    struct SVGTextAnchorAttribute : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("text-anchor", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGTextAnchorAttribute>();
                node->loadFromAttributes(attrs);
                return node;
                });
        }


        TXTALIGNMENT fValue{ TXTALIGNMENT::LEFT };

        SVGTextAnchorAttribute() :SVGVisualProperty(nullptr)
        {
            id("text-anchor");
        }

        int value() const { return fValue; }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            bool success = getEnumValue(SVGTextAnchor, inChunk, (int&)fValue);
            set(success);

            return success;
        }

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->textAnchor(fValue);
        }
    };
}

namespace waavs {
    struct SVGFontSize : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("font-size", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGFontSize>(nullptr); 
                node->loadFromAttributes(attrs);
                return node; 
                });
        }
        
        
        SVGDimension dimValue{};
        double fValue{ 16.0 };

        SVGFontSize(IAmGroot* inMap) 
            : SVGVisualProperty(inMap) 
        {
			id("font-size");
        }

        SVGFontSize& operator=(const SVGFontSize& rhs)
		{
			dimValue = rhs.dimValue;
			fValue = rhs.fValue;
			
            return *this;
		}

		double value() const { return fValue; }
        
        
        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->fontSize(fValue);
        }

        void bindToGroot(IAmGroot* groot, SVGViewable* container) noexcept override
        {
            if (nullptr == groot)
                return;
            
            if (dimValue.isSet())
			    fValue = dimValue.calculatePixels(16, 0, groot->dpi());
            
            needsBinding(false);
        }
        
        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
			if (!inChunk)
				return false;
            
            dimValue.loadFromChunk(inChunk);

            if (!dimValue.isSet())
                return false;
            
            needsBinding(true);

            set(true);
                
            return true;
        }


    };

    //========================================================
    // SVGFontFamily
    // This is a fairly complex attribute, as the family might be
    // a font family name, or it might be a class, such as 'sans-serif'
    // attribute name="font-style" type="string" default="normal"
    // BUGBUG
    struct SVGFontFamily : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("font-family", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGFontFamily>(nullptr); 
                node->loadFromAttributes(attrs);
                return node; 
                });
        }
        


        ByteSpan fValue{};

        SVGFontFamily(IAmGroot* inMap) : SVGVisualProperty(inMap) { id("font-family"); }

        SVGFontFamily& operator=(const SVGFontFamily& rhs) = delete;


		const ByteSpan& value() const { return fValue; }
        
        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->fontFamily(fValue);
        }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            if (!inChunk)
				return false;
            
            fValue = inChunk;
            //fValue = toString(chunk_trim(inChunk, xmlwsp));
            set(true);
            
			return true;
        }

    };

	//========================================================
	// SVGFontStyle
	// attribute name="font-style" type="string" default="normal"
    //========================================================
    struct SVGFontStyleAttribute : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("font-style", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGFontStyleAttribute>();
                node->loadFromAttributes(attrs);
                return node;
                });
        }
        
        
        BLFontStyle fValue{ BL_FONT_STYLE_NORMAL };

        SVGFontStyleAttribute() :SVGVisualProperty(nullptr) 
        { 
            id("font-style");  
            set(false); 
            needsBinding(false); 
        }
        
        int value() const {return fValue;}
        
        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            
            bool success = getEnumValue(SVGFontStyle, inChunk, (int &)fValue);
			set(success);
            
			return success;
        }

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->fontStyle(fValue);
        }
    };
    
    struct SVGFontWeightAttribute : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("font-weight", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGFontWeightAttribute>();
                node->loadFromAttributes(attrs);
                return node;
                });
        }
        
        BLFontWeight fWeight{ BL_FONT_WEIGHT_NORMAL };
        
        SVGFontWeightAttribute() :SVGVisualProperty(nullptr) { id("font-weight"); }

		BLFontWeight value() const { return fWeight; }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            needsBinding(false);
			bool success = getEnumValue(SVGFontWeight, inChunk, (int &)fWeight);
            set(success);

			return success;
		}

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->fontWeight(value());
        }
	};

    struct SVGFontStretchAttribute : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("font-stretch", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGFontStretchAttribute>();
                node->loadFromAttributes(attrs);
                return node;
                });
        }
        
        BLFontStretch fValue{ BL_FONT_STRETCH_NORMAL };

        SVGFontStretchAttribute() :SVGVisualProperty(nullptr) { id("font-stretch"); }

        BLFontStretch value() const { return fValue; }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
			bool success = getEnumValue(SVGFontStretch, inChunk, (int&)fValue);
			set(success);
            
            return success;

        }

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->fontStretch(value());
        }
        
    };
    
}




namespace waavs {
    //
    // htt://www.w3.org/TR/SVG11/feature#PaintAttribute
    // color, 
    // fill, fill-opacity,
    // fill-rule, 
    // stroke, stroke-opacity,
    // stroke-width
    // stroke-dasharray, stroke-dashoffset, 
    // stroke-linecap, stroke-linejoin, stroke-miterlimit,  

    struct SVGPaintAttribute : public SVGVisualProperty
    {
        BLVar fPaintVar{};

        SVGPaintAttribute(IAmGroot* groot) :SVGVisualProperty(groot) {}

        const BLVar getVariant() noexcept override
        {
            return fPaintVar;
        }
    };

    //=====================================================
    // SVG Paint
    // General base class for paint.  Other kinds of paints
    // such as fill, stroke, stop-color, descend from this
    //=====================================================
    struct SVGPaint : public SVGPaintAttribute
    {
        ByteSpan fPaintReference{};


        SVGPaint(IAmGroot* iMap) : SVGPaintAttribute(iMap) {}
        SVGPaint(const SVGPaint& other) = delete;



        //
        // resolvePaint()
        // We have a reference to something this is supposed to be our 
        // paint.  Try to retrieve it, and get it's variant.
        // BUGBUG - Ideally, thie is where we could do ObjectBBox binding
        // 
        virtual void resolvePaint(IAmGroot* groot, SVGViewable* container)
        {
            if (!fPaintReference)
                return;


            if (chunk_starts_with_cstr(fPaintReference, "url("))
            {
                auto node = groot->findNodeByUrl(fPaintReference);

                if (nullptr == node)
                    return;


                // Tell the referant node to resolve itself
                node->bindToGroot(groot, container);

                const BLVar& aVar = node->getVariant();

                auto res = blVarAssignWeak(&fPaintVar, &aVar);
                if (res != BL_SUCCESS)
                    return;

                set(true);
            }
        }



        // called when we have a reference to something
        void bindToGroot(IAmGroot* groot, SVGViewable* container) noexcept override
        {
            if (!needsBinding()) {
                return;
            }

            resolvePaint(groot, nullptr);

            needsBinding(false);
        }


        void update(IAmGroot* groot) override
        {
            ByteSpan ref = rawValue();

            if (chunk_starts_with_cstr(ref, "url("))
            {
                if (groot != nullptr) {
                    auto node = groot->findNodeByUrl(ref);
                    if (nullptr != node)
                    {
                        node->update(groot);
                    }
                }
            }
        }

        void setOpacity(double opacity)
        {
            uint32_t outValue;
            if (BL_SUCCESS == blVarToRgba32(&fPaintVar, &outValue))
            {
                BLRgba32 newColor(outValue);
                newColor.setA((uint32_t)(opacity * 255));
                fPaintVar = newColor;
                //blVarAssignRgba32(&fPaintVar, newColor.value);
            }
        }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            static const char* rgbStr = "rgb(";
            static const char* rgbaStr = "rgba(";
            static const char* rgbStrCaps = "RGB(";
            static const char* rgbaStrCaps = "RGBA(";
            static const char* hslStr = "hsl(";
            static const char* hslaStr = "hsla(";

            ByteSpan str = inChunk;


            size_t len = 0;

            // First check to see if it's a lookup by 'url'
            // if it is, then register our desire to do a lookup
            // and finish for now.
            if (chunk_starts_with_cstr(str, "url("))
            {
                fPaintReference = str;
                //fNeedsResolving = true;
                needsBinding(true);

                return true;
            }

            BLRgba32 c(128, 128, 128);
            len = str.size();
            if ((len >= 1) && (*str == '#'))
            {
                c = parseColorHex(str);
                fPaintVar = c;
                set(true);
            }
            else if (str.startsWith(rgbStr) ||
                str.startsWith(rgbaStr) ||
                str.startsWith(rgbaStrCaps) ||
                str.startsWith(rgbStrCaps))
            {
                parseColorRGB(str, c);
                fPaintVar = c;
                set(true);
            }
            else if (str.startsWith(hslStr) ||
                str.startsWith(hslaStr))
            {
                c = parseColorHsl(str);
                fPaintVar = c;

                set(true);
            }
            else {
                if (str == "none") {
                    fPaintVar = BLVar::null();
                    set(true);
                }
                else if ((str == "context-stroke") || (str == "context-fill"))
                {
                    set(false);
                }
                else if ((str == "inherit") || (str == "currentColor"))
                {
                    // Take on whatever color value was previously set
                    // somewhere in the tree
                    set(false);
                }
                else {
                    c = getSVGColorByName(str);
                    fPaintVar = c;

                    set(true);
                }
            }

            return true;
        }
    };
}

namespace waavs {
    struct SVGColorPaint : public SVGPaint
    {
        static void registerFactory() {
            registerSVGAttribute("color", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGColorPaint>(nullptr);
                node->loadFromAttributes(attrs);
                return node;
                });
        }


        SVGColorPaint(IAmGroot* root) : SVGPaint(root) { id("color"); }


        bool loadFromAttributes(const XmlAttributeCollection& attrs)
        {
            if (!SVGPaint::loadFromAttributes(attrs))
            {
                return false;
            }

            return true;
        }

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->defaultColor(getVariant());
        }

    };
}

namespace waavs {
    //
    //
    struct SVGFillPaint : public SVGPaint
    {
        static void registerFactory() {
            registerSVGAttribute("fill", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGFillPaint>(nullptr);
                node->loadFromAttributes(attrs);
                return node;
                });
        }


        SVGFillPaint(IAmGroot* root) : SVGPaint(root) { id("fill"); }


        bool loadFromAttributes(const XmlAttributeCollection& attrs)
        {
            if (!SVGPaint::loadFromAttributes(attrs))
            {
                return false;
            }

			return true;
        }
        
        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->fill(getVariant());
        }

    };

    //=========================================================
    // SVGFillRule
    //=========================================================
    static bool parseFillRule(const ByteSpan& inChunk, BLFillRule& value)
    {
        if (inChunk == "nonzero")
            value = BL_FILL_RULE_NON_ZERO;
        else if (inChunk == "evenodd")
            value = BL_FILL_RULE_EVEN_ODD;
        else
            return false;

        return true;
    }
    
    struct SVGFillRuleAttribute : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("fill-rule", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGFillRuleAttribute>(nullptr);
                node->loadFromAttributes(attrs);
                return node;
                });
        }


        BLFillRule fValue{ BL_FILL_RULE_EVEN_ODD };

        SVGFillRuleAttribute(IAmGroot* iMap) : SVGVisualProperty(iMap) { id("fill-rule"); }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            needsBinding(false);

			bool success = getEnumValue(SVGFillRule, inChunk, (int &)fValue);
            set(success);
            
            return success;
        }

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            if (isSet())
                ctx->fillRule(fValue);
        }
    };
}





namespace waavs {
    
    struct SVGStrokePaint : public SVGPaint
    {
        static void registerFactory() {
            registerSVGAttribute("stroke", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGStrokePaint>(nullptr); 
                node->loadFromAttributes(attrs);
                return node; 
                });
        }
        

        SVGStrokePaint(IAmGroot* root) : SVGPaint(root) { id("stroke"); }

		void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
		{
            ctx->stroke(getVariant());
		}

    };


    //=========================================================
    // SVGStrokeWidth
    //=========================================================

    struct SVGStrokeWidth : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("stroke-width", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGStrokeWidth>(nullptr); 
                node->loadFromAttributes(attrs);  
                return node; 
                });
        }

        double fWidth{ 1.0 };

        SVGStrokeWidth(IAmGroot* iMap) : SVGVisualProperty(iMap) { id("stroke-width"); }

        SVGStrokeWidth(const SVGStrokeWidth& other) = delete;
        SVGStrokeWidth& operator=(const SVGStrokeWidth& rhs) = delete;

        
        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->strokeWidth(fWidth);
        }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            if (!inChunk)
                return false;
            
            ByteSpan s = inChunk;
            if (!readNumber(s, fWidth))
                return false;
            
            set(true);
            
            return true;
        }


    };

    //=========================================================
    ///  SVGStrokeMiterLimit
    /// A visual property to set the miter limit for a stroke
    //=========================================================
    struct SVGStrokeMiterLimit : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("stroke-miterlimit", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGStrokeMiterLimit>(nullptr); node->loadFromAttributes(attrs);  return node; });
        }


        double fMiterLimit{ 4.0 };

        SVGStrokeMiterLimit(IAmGroot* iMap) : SVGVisualProperty(iMap) { id("stroke-miterlimit"); }

        SVGStrokeMiterLimit(const SVGStrokeMiterLimit& other) = delete;
        SVGStrokeMiterLimit& operator=(const SVGStrokeMiterLimit& rhs) = delete;

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->strokeMiterLimit(fMiterLimit);
        }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            ByteSpan s = inChunk;
            
            if (!readNumber(s, fMiterLimit))
                return false;
            
            fMiterLimit = clamp(fMiterLimit, 1.0, 10.0);

            set(true);
            needsBinding(false);

            return true;
        }


    };
    
    //=========================================================
    // SVGStrokeLineCap
    //=========================================================

    
    struct SVGStrokeLineCap : public SVGVisualProperty
    {
        static void registerFactory()
        {
            registerSVGAttribute("stroke-linecap", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGStrokeLineCap>(nullptr, "stroke-linecap"); node->loadFromAttributes(attrs);  return node; });
            registerSVGAttribute("stroke-linecap-start", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGStrokeLineCap>(nullptr, "stroke-linecap-start"); node->loadFromAttributes(attrs);  return node; });
            registerSVGAttribute("stroke-linecap-end", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGStrokeLineCap>(nullptr, "stroke-linecap-end"); node->loadFromAttributes(attrs);  return node; });
        }


        BLStrokeCap fLineCap{ BL_STROKE_CAP_BUTT };
        BLStrokeCapPosition fLineCapPosition{};
        bool fBothCaps{ true };

        SVGStrokeLineCap(IAmGroot* iMap, const ByteSpan& name) : SVGVisualProperty(iMap)
        {
            id(name);
            
            if (name == "stroke-linecap")
                fBothCaps = true;
            else if (name == "stroke-linecap-start")
            {
                fBothCaps = false;
                fLineCapPosition = BL_STROKE_CAP_POSITION_START;
            }
            else if (name == "stroke-linecap-end")
            {
                fBothCaps = false;
                fLineCapPosition = BL_STROKE_CAP_POSITION_END;
            }
        }

        SVGStrokeLineCap(const SVGStrokeLineCap& other) = delete;
        SVGStrokeLineCap& operator=(const SVGStrokeLineCap& rhs) = delete;

        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            if (fBothCaps) {
                ctx->strokeCaps(fLineCap);
            }
            else {
                ctx->strokeCap(fLineCap, fLineCapPosition);
            }

        }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            needsBinding(false);
            
            bool success = getEnumValue(SVGLineCaps, inChunk, (int&)fLineCap);
            set(success);
            
			return success;
        }

    };

    //=========================================================
    // SVGStrokeLineJoin
    // A visual property to set the line join for a stroke
    //=========================================================
    struct SVGStrokeLineJoin : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("stroke-linejoin", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGStrokeLineJoin>(nullptr); node->loadFromAttributes(attrs);  return node; });
        }

        BLStrokeJoin fLineJoin{ BL_STROKE_JOIN_MITER_BEVEL };

        SVGStrokeLineJoin(IAmGroot* iMap) : SVGVisualProperty(iMap) { id("stroke-linejoin"); }
        SVGStrokeLineJoin(const SVGStrokeLineJoin& other) = delete;
        SVGStrokeLineJoin& operator=(const SVGStrokeLineJoin& rhs) = delete;


        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
            ctx->strokeJoin(fLineJoin);
        }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            bool success = getEnumValue(SVGLineJoin, inChunk, (int &)fLineJoin);
            set(success);

            return success;
        }

    };
}



namespace waavs {

    //======================================================
    // SVGViewbox
    // A document may or may not have this property
    //======================================================

    struct SVGViewbox : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("viewBox", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGViewbox>(nullptr); node->loadFromAttributes(attrs);  return node; });

        }

        
        BLRect fRect{};
        
        SVGViewbox() :SVGVisualProperty(nullptr) { id("viewBox"); }
        SVGViewbox(IAmGroot* iMap) :SVGVisualProperty(iMap) {}
        
        SVGViewbox(const SVGViewbox& other) = delete;
        SVGViewbox& operator=(const SVGViewbox& rhs) = delete;

        // This will translate relative to the current x, y position
        void translateBy(double dx, double dy)
        {
            fRect.x += dx;
            fRect.y += dy;
        }

        // This does a scale relative to a given point
        // It will also do the translation at the same time
        void scaleBy(double sx, double sy, double centerx, double centery)
        {            
            fRect.x = centerx + (fRect.x - centerx) * sx;
            fRect.y = centery + (fRect.y - centery) * sy;
            fRect.w *= sx;
            fRect.h *= sy;
        }

        double x() const { return fRect.x; }
        double y() const { return fRect.y; }
        double width() const { return fRect.w; }
        double height() const { return fRect.h; }

        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
			if (!parseViewBox(inChunk, fRect))
                return false;

            set(true);
            return true;
        }

    };


}





namespace waavs {


    //
    // SVGOrient
    // Determines how the marker should be oriented, and ultimately
    // what angle of rotation should be applied before drawing
    // 
    struct SVGOrient
    {
        double fAngle{ 0 };
        MarkerOrientation fOrientation{ MarkerOrientation::MARKER_ORIENT_AUTO };


        SVGOrient(IAmGroot* groot) {}

        // In order to calculate the angle, we need the path
        // so we can determine the tangent at the start or end
        bool loadFromChunk(const ByteSpan& inChunk)
        {
            ByteSpan s = inChunk;
            s = chunk_skip_wsp(s);

            if (!s)
                return false;

            if (!parseMarkerOrientation(s, fOrientation))
                return false;

            switch (fOrientation)
            {
            case MarkerOrientation::MARKER_ORIENT_AUTO:
                return true;
                break;


            case MarkerOrientation::MARKER_ORIENT_AUTOSTARTREVERSE:
            {
                return true;

            } break;

            case MarkerOrientation::MARKER_ORIENT_ANGLE:
            default:
            {
                SVGAngleUnits units{ SVGAngleUnits::SVG_ANGLETYPE_UNKNOWN };
                return parseAngle(s, fAngle, units);
            } break;

            }

            return true;
        }


        // Given the specified orientation, and a path, calculate the angle
        // of rotation for the marker
        // return the value in radians
		

        
        double calcRadians(MarkerPosition pos, const BLPoint& p1, const BLPoint& p2, const BLPoint& p3) const
        {
            if (fOrientation == MarkerOrientation::MARKER_ORIENT_ANGLE)
            {
                // fAngle is already in radians
                return fAngle;
            }
            
			// Calculate the angle based on the tangent
            double ang1 = radians_normalize(atan2(p2.y - p1.y, p2.x-p1.x));
			double ang2 = radians_normalize(atan2(p2.y - p3.y, p2.x-p3.x));

            double angle = ang1;

            

            switch (pos)
            {
            case MARKER_POSITION_START:
            {
                if (fOrientation == MarkerOrientation::MARKER_ORIENT_AUTOSTARTREVERSE)
                    angle = angle + waavs::pi;
            }
            break;

            case MARKER_POSITION_MIDDLE:
                angle = (ang2 - ang1) / 2.0;
                break;

            case MARKER_POSITION_END:
                //if (fOrientation == MarkerOrientation::MARKER_ORIENT_AUTOSTARTREVERSE)
                //    return ang1 + waavs::pi;
                break;
            }
            
            //printf("calcRadians: %3.2f (%3.2f, %3.2f)\n", angle, ang1, ang2);

            return radians_normalize(angle);
        }

        
        double calculateRadians(MarkerPosition pos, const BLPoint& p1, const BLPoint& p2, const BLPoint& p3) const
        {
            if (fOrientation == MarkerOrientation::MARKER_ORIENT_ANGLE)
            {
                // fAngle is already in radians
                return fAngle;
            }

            
            // Calculate the angle based on tangent
			double diffx1 = p2.x - p1.x;
			double diffy1 = p2.y - p1.y;
			//double diffx2 = p3.x - p2.x;
			//double diffy2 = p3.y - p2.y;
            
            //double diffx1 = p1.x - p2.x;
            //double diffy1 = p1.y - p2.y;
            double diffx2 = p3.x - p2.x;
            double diffy2 = p3.y - p2.y;

            double ang = 0;
            double ang1 = std::atan2(diffy1, diffx1);
            double ang2 = std::atan2(diffy2, diffx2);
            
            switch (pos)
            {
                case MARKER_POSITION_START: 
                {
                    if (fOrientation == MarkerOrientation::MARKER_ORIENT_AUTOSTARTREVERSE)
                        ang = ang1 + waavs::pi;
                }
                break;
                
                case MARKER_POSITION_MIDDLE:
                    ang = (ang1 + ang2) / 2.0;
                break;

                case MARKER_POSITION_END:
                    //if (fOrientation == MarkerOrientation::MARKER_ORIENT_AUTOSTARTREVERSE)
                    //    return ang1 + waavs::pi;
                    
                   ang = ang1;
                break;
            }

            
            return ang;
        }
    };
}

//
// Markers
//
namespace waavs {
    struct SVGMarkerAttribute : public SVGVisualProperty
    {
        
        static void registerMarkerFactory() {
            registerSVGAttribute("marker", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGMarkerAttribute>(nullptr,"marker"); node->loadFromAttributes(attrs);  return node; });
            registerSVGAttribute("marker-start", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGMarkerAttribute>(nullptr,"marker-start"); node->loadFromAttributes(attrs);  return node; });
            registerSVGAttribute("marker-mid", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGMarkerAttribute>(nullptr, "marker-mid"); node->loadFromAttributes(attrs);  return node; });
            registerSVGAttribute("marker-end", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGMarkerAttribute>(nullptr, "marker-end"); node->loadFromAttributes(attrs);  return node; });
        }

        std::shared_ptr<SVGViewable> fWrappedNode = nullptr;

        
        SVGMarkerAttribute(IAmGroot* groot, const ByteSpan& name) : SVGVisualProperty(groot) { id(name); }
        
		std::shared_ptr<SVGViewable> markerNode() const
		{
            return fWrappedNode;
		}
        
        void bindToGroot(IAmGroot* groot, SVGViewable* container) noexcept override
        {
            
            if (chunk_starts_with_cstr(rawValue(), "url("))
            {
                fWrappedNode = groot->findNodeByUrl(rawValue());

                if (fWrappedNode != nullptr)
                {
					fWrappedNode->bindToGroot(groot, container);
                    set(true);
                }
            }
            needsBinding(false);
        }
        
        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
			autoDraw(false); // we mark it as invisible, because we don't want it drawing when attributes are drawn
			// we only want it to draw when we're drawing during polyline/polygon drawing
            needsBinding(true);
            
            return true;
        }
        
        void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
        {
			if (fWrappedNode)
				fWrappedNode->draw(ctx, groot);
        }
    };
}

namespace waavs {
    //======================================================
    // SVGClipAttribute
    // This is the attribute that can be connected to some
    // shape that's being drawn.  Whatever is doing the drawing
    // should call getVariant() to retrieve the BLImage which
    // represents the clipPath 
	//======================================================
    struct SVGClipPathAttribute : public SVGVisualProperty
    {
        static void registerFactory()
        {
            registerSVGAttribute("clip-path", [](const XmlAttributeCollection& attrs) {
                auto node = std::make_shared<SVGClipPathAttribute>(nullptr);
                node->loadFromAttributes(attrs);

                return node;
                });

        }


        std::shared_ptr<SVGViewable> fClipNode{ nullptr };
        BLVar fClipVar;
        
        
        SVGClipPathAttribute(IAmGroot* groot) : SVGVisualProperty(groot) { id("clip-path"); }


        bool loadFromUrl(IAmGroot* groot, const ByteSpan& inChunk)
        {
            if (nullptr == groot)
                return false;

            ByteSpan str = inChunk;

            fClipNode = groot->findNodeByUrl(inChunk);

            if (fClipNode == nullptr) {
                set(false);
                return false;
            }

            // pull out the color value
            // BUGBUG - this will not always be the case
            // as what we point to might be a gradient or pattern
            if (fClipNode->needsBinding())
                fClipNode->bindToGroot(groot, nullptr);

            set(true);

            return true;
        }

        
		const BLVar getVariant() noexcept override
		{
            if (fClipNode == nullptr)
                return fClipVar;
            
            // BUGBUG
            //return fClipNode->getVariant();
            return BLVar::null();
		}

        // Let's get a connection to our referenced thing
        void bindToGroot(IAmGroot* groot, SVGViewable* container) noexcept override
        {
            ByteSpan str = rawValue();

            // Just load the referenced node at this point
            
            if (chunk_starts_with_cstr(str, "url("))
            {
                loadFromUrl(groot, str);
            }

            needsBinding(false);
        }
        
        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            needsBinding(true);
            set(true);
            
            return true;
        }
    };
}
    
namespace waavs {
    
    struct SVGVectorEffectAttribute : public SVGVisualProperty
    {
        static void registerFactory() {
            registerSVGAttribute("vector-effect", [](const XmlAttributeCollection& attrs) {auto node = std::make_shared<SVGVectorEffectAttribute>(nullptr); node->loadFromAttributes(attrs);  return node; });
        }

        VectorEffectKind fEffectKind{ VECTOR_EFFECT_NONE };

        
        SVGVectorEffectAttribute(IAmGroot* groot) : SVGVisualProperty(groot) { id("vector-effect"); }
        
        bool loadSelfFromChunk(const ByteSpan& inChunk) override
        {
            needsBinding(false);
            
			bool success = getEnumValue(SVGVectorEffect, inChunk, (int &)fEffectKind);
            set(success);
            
            return success;
        }

		void drawSelf(IRenderSVG* ctx, IAmGroot* groot) override
		{
			if (VECTOR_EFFECT_NON_SCALING_STROKE == fEffectKind)
			{
                ctx->strokeBeforeTransform(true);
			}
		}
    };
}



