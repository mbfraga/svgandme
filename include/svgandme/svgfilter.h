#pragma once

#include "svgstructuretypes.h"


#include <string>
#include <array>
#include <functional>
#include <unordered_map>


// Elements related to filters
// filter			- compound


// feBlend			- single
// feColorMatrix	- single
// feComponentTransfer- single
// feComposite		- single
// feConvolveMatrix	- single
// feDiffuseLighting- single
// feDisplacementMap- single
// feDistantLight	- single
// feFlood			- single
// feGaussianBlur	- single
// feImage			- single
// feMerge			- compound
// feMergeNode		- single
// feMorphology		- single
// feOffset			- single
// fePointLight		- single
// feSpecularLighting- single
// feSpotLight		- single
// feTile			- single
// feTurbulence		- single



// feFuncR			- single
// feFuncG			- single
// feFuncB			- single
// feFuncA			- single
//

namespace waavs {

	//
	// filter
	//
	struct SVGFilterElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["filter"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFilterElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
			};
		}
		
		static void registerFactory()
		{
			gSVGGraphicsElementCreation["filter"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFilterElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				
				return node;
			};

			registerSingularNode();
		}

		// filters need to setup an execution environment.  
		SVGDimension fX{};
		SVGDimension fY{};
		SVGDimension fWidth{};
		SVGDimension fHeight{};
		
		// dictionary of images used in the filter
		std::unordered_map<std::string, BLImage> fFilterImages;
		
		
		SVGFilterElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(false);
		}

		bool addImage(const std::string &name, BLImage &img)
		{
			auto it = fFilterImages.find(name);
			if (it != fFilterImages.end())
				return false;

			fFilterImages[name] = img;
			return true;
		}

		bool getImage(const std::string& name, BLImage &img)
		{
			auto it = fFilterImages.find(name);
			if (it == fFilterImages.end())
				return false;

			img = it->second;
			return true;
		}

		virtual bool addNode(std::shared_ptr < SVGVisualNode > node, IAmGroot* groot)
		{
			// if superclass fails to add the node, then forget it
			if (!SVGGraphicsElement::addNode(node, groot))
				return false;
			
			//printf("SVGFeFilter.addNode(%s)\n", node->id().c_str());
			
			return true;
		}

		//void loadVisualProperties(const XmlAttributeCollection& attrs, IAmGroot* groot) override
		void resolvePosition(IAmGroot* groot, SVGViewable* container) override
		{
			//SVGGraphicsElement::loadVisualProperties(attrs, groot);

			fX.loadFromChunk(getAttribute("x"));
			fY.loadFromChunk(getAttribute("y"));
			fWidth.loadFromChunk(getAttribute("width"));
			fHeight.loadFromChunk(getAttribute("height"));

		}
		
	};

	//
	// feBlend
	//
	struct SVGFeBlendElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feBlend"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeBlendElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
				};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feBlend"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeBlendElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				node->visible(false);
				
				return node;
				};

			registerSingularNode();
		}



		SVGFeBlendElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}
	};
	
	//
	// feComponentTransfer
	//
	struct SVGFeComponentTransferElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feComponentTransfer"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeComponentTransferElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
				};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feComponentTransfer"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeComponentTransferElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				return node;
				};

			registerSingularNode();
		}



		SVGFeComponentTransferElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}

	};
	
	
	//
	// feComposite
	//
	struct SVGFeCompositeElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feComposite"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeCompositeElement>(groot);
				node->loadFromXmlElement(elem, groot);
				
				return node;
			};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feComposite"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeCompositeElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				
				return node;
			};
			
			registerSingularNode();
		}



		SVGFeCompositeElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}

	};

	//
	// feColorMatrix
	//
	struct SVGFeColorMatrixElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feColorMatrix"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeColorMatrixElement>(groot);
				node->loadFromXmlElement(elem, groot);
				
				return node;
			};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feColorMatrix"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeColorMatrixElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				
				return node;
			};

			registerSingularNode();
		}


		SVGFeColorMatrixElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}

	};

	//
	// feConvolveMatrix
	//
	struct SVGFeConvolveMatrixElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feConvolveMatrix"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeConvolveMatrixElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
				};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feConvolveMatrix"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeConvolveMatrixElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				return node;
				};

			registerSingularNode();
		}


		SVGFeConvolveMatrixElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}

	};


	//
	// feDiffuseLighting
	//
	struct SVGFeDiffuseLightingElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feDiffuseLighting"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeDiffuseLightingElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
				};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feDiffuseLighting"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeDiffuseLightingElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				return node;
				};

			registerSingularNode();
		}


		SVGFeDiffuseLightingElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}

	};
	
	
	//
	// feDisplacementMap
	//
	struct SVGFeDisplacementMapElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feDisplacementMap"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeDisplacementMapElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
				};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feDisplacementMap"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeDisplacementMapElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				return node;
				};

			registerSingularNode();
		}


		SVGFeDisplacementMapElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}

	};

	//
	// feDistantLight
	//
	struct SVGFeDistantLightElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feDistantLight"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeDistantLightElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
				};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feDistantLight"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeDistantLightElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				
				return node;
				};

			registerSingularNode();
		}


		SVGFeDistantLightElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}

	};
	
	//
	// feFlood
	//
	struct SVGFeFloodElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feFlood"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeFloodElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
				};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feFlood"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeFloodElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				node->visible(false);
				
				return node;
				};

			registerSingularNode();
		}



		SVGFeFloodElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}
	};

	
	//
	// feGaussianBlur
	//
	struct SVGFeGaussianBlurElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feGaussianBlur"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeGaussianBlurElement>(groot);
				node->loadFromXmlElement(elem, groot);
				
				return node;
			};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feGaussianBlur"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeGaussianBlurElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				node->visible(false);
				
				return node;
			};
			
			registerSingularNode();
		}
		

		SVGDimension fStdDeviation;
		
		SVGFeGaussianBlurElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}

		void resolvePosition(IAmGroot* groot, SVGViewable* container) override
		{
			fStdDeviation.loadFromChunk(getAttribute("stdDeviation"));
		}
	};

	//
	// feOffset
	//
	struct SVGFeOffsetElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feOffset"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeOffsetElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
				};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feOffset"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeOffsetElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				node->visible(false);
				
				return node;
				};

			registerSingularNode();
		}


		SVGFeOffsetElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}
	};
	
	
	//
	// feTurbulence
	//
	struct SVGFeTurbulenceElement : public SVGGraphicsElement
	{
		static void registerSingularNode()
		{
			gShapeCreationMap["feTurbulence"] = [](IAmGroot* groot, const XmlElement& elem) {
				auto node = std::make_shared<SVGFeTurbulenceElement>(groot);
				node->loadFromXmlElement(elem, groot);

				return node;
			};
		}

		static void registerFactory()
		{
			gSVGGraphicsElementCreation["feTurbulence"] = [](IAmGroot* groot, XmlElementIterator& iter) {
				auto node = std::make_shared<SVGFeTurbulenceElement>(groot);
				node->loadFromXmlIterator(iter, groot);
				
				return node;
			};

			registerSingularNode();
		}



		SVGFeTurbulenceElement(IAmGroot* aroot)
			: SVGGraphicsElement(aroot)
		{
			isStructural(true);
		}
	};
}
