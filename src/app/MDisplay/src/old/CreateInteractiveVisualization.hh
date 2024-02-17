#ifndef CREATE_INTERACTIVE_VISUALIZATION_H
#define CREATE_INTERACTIVE_VISUALIZATION_H

#include <memory>
#include "XML.hh"


// using namespace std;



std::tuple<std::string, std::string, std::string> createDetectorAsSvgString() 
{ // TODO: check position in yz plane!

	std::string svg_group_detector_xz;
	std::string svg_group_detector_yz;
	std::string svg_group_detector_xy;
	//// xz
	std::string reflector_elements = "<g id=\"detector-xz\" stroke-width=\"0.3\" stroke=\"black\">\n";
	for (int i = 0; i <= 192; ++i) {
		int cube_x_pos = -960.0 + i * 10;
		std::string cube_x_pos_str = std::to_string(cube_x_pos);
		// XMLObject *line = new XMLObject("line", "", {
		// 	{"class", "reflector"},
		// 	{"x1", cube_x_pos_str}, {"x2", cube_x_pos_str},
		// 	{"y1", "-920"}, {"y2", "920"}
		// });
		// reflector_elements += line->to_str();
		// delete line;
		std::string line = "<line class=\"reflector\" x1=\""\
						   + cube_x_pos_str\
						   + "\" x2=\""\
						   + cube_x_pos_str\
						   + "\" y1=\"-920.0\" y2=\"920.0\"/>\n";
		reflector_elements += line;
	}

	for (int i = 0; i <= 184; ++i) {
		int cube_z_pos = -920.0 + i * 10;
		std::string cube_z_pos_str = std::to_string(cube_z_pos);
		// XMLObject *line = new XMLObject("line", "", {
		// 	{"class", "reflector"},
		// 	{"y1", cube_z_pos_str}, {"y2", cube_z_pos_str},
		// 	{"x1", "-960"}, {"x2", "960"}
		// });
		// reflector_elements += line->to_str();
		// delete line;
		std::string line = "<line class=\"reflector\" y1=\""\
						   + cube_z_pos_str\
						   + "\" y2=\""\
						   + cube_z_pos_str\
						   + "\" x1=\"-960.0\" x2=\"960.0\"/>\n";
		reflector_elements += line;
	}

	reflector_elements += "</g>\n";

	svg_group_detector_xz = reflector_elements;

	//// yz
	reflector_elements = "<g id=\"detector-yz\" stroke-width=\"0.3\" stroke=\"black\">";
	for (int i = 0; i <= 56; ++i) {
		int cube_y_pos = -280.0 + i * 10;
		std::string cube_y_pos_str = std::to_string(cube_y_pos);
		std::string line = "<line class=\"reflector\" x1=\""\
						   + cube_y_pos_str\
						   + "\" x2=\""\
						   + cube_y_pos_str\
						   + "\" y1=\"-920.0\" y2=\"920.0\"/>\n";
		reflector_elements += line;
	}

	for (int i = 0; i <= 184; ++i) {
		int cube_z_pos = -920.0 + i * 10;
		std::string cube_z_pos_str = std::to_string(cube_z_pos);
		std::string line = "<line class=\"reflector\" y1=\""\
						   + cube_z_pos_str\
						   + "\" y2=\""\
						   + cube_z_pos_str\
						   + "\" x1=\"-280.0\" x2=\"280.0\"/>\n";
		reflector_elements += line;
	}

	reflector_elements += "</g>\n";

	svg_group_detector_yz = reflector_elements;

	//// xy
	reflector_elements = "<g id=\"detector-xy\" stroke-width=\"0.3\" stroke=\"black\">";
	for (int i = 0; i <= 192; ++i) {
		int cube_x_pos = -960.0 + i * 10;
		std::string cube_x_pos_str = std::to_string(cube_x_pos);
		std::string line = "<line class=\"reflector\" x1=\""\
						   + cube_x_pos_str\
						   + "\" x2=\""\
						   + cube_x_pos_str\
						   + "\" y1=\"-280.0\" y2=\"280.0\"/>\n";
		reflector_elements += line;
	}

	for (int i = 0; i <= 56; ++i) {
		int cube_y_pos = -280.0 + i * 10;
		std::string cube_y_pos_str = std::to_string(cube_y_pos);
		std::string line = "<line class=\"reflector\" y1=\""\
						   + cube_y_pos_str\
						   + "\" y2=\""\
						   + cube_y_pos_str\
						   + "\" x1=\"-960.0\" x2=\"960.0\"/>\n";
		reflector_elements += line;
	}

	reflector_elements += "</g>\n";

	svg_group_detector_xy = reflector_elements;

	// return {svg_group_detector_xz, svg_group_detector_yz, svg_group_detector_xy}; /// probably a clang bug (according to StackOverFlow)
	return std::make_tuple(svg_group_detector_xz, svg_group_detector_yz, svg_group_detector_xy); /// probably a clang bug (according to StackOverFlow)
}


std::tuple<XMLObject*, XMLObject*, XMLObject*> createDetectorAsXMLObject(
	int NxCubes = 192, float xStart = -960.0,
	int NyCubes = 56 , float yStart = -280.0,
	int NzCubes = 184, float zStart = -920.0,
	string strokeColor = "white")
{
	using namespace std;

	string xStartStr = to_string(int(xStart));
	string xStopStr = to_string(int(NxCubes * 10.0 + xStart));
	string yStartStr = to_string(int(yStart));
	string yStopStr = to_string(int(NyCubes * 10.0 + yStart));
	string zStartStr = to_string(int(zStart));
	string zStopStr = to_string(int(NzCubes * 10.0 + zStart));

	/// for xz plane ///
	XMLObject * xz_reflector_elements = new XMLObject("g", "", {{"id", "detector-xz"}, {"stroke-width", "0.3"}, {"stroke", strokeColor}});
	for (int i = 0; i <= NxCubes; ++i)
	{
		int cube_x_pos = xStart + i * 10;
		string cube_x_pos_str = std::to_string(cube_x_pos);
		[[maybe_unused]] XMLObject * line = new XMLObject("line", "", {
			{"class", "reflector"}, {"x1", cube_x_pos_str}, {"x2", cube_x_pos_str}, {"y1", zStartStr}, {"y2", zStopStr}
		}, xz_reflector_elements);
	}
	for (int i = 0; i <= NzCubes; ++i) {
		int cube_z_pos = zStart + i * 10;
		std::string cube_z_pos_str = std::to_string(cube_z_pos);
		[[maybe_unused]] XMLObject *line = new XMLObject("line", "", {
			{"class", "reflector"}, {"y1", cube_z_pos_str}, {"y2", cube_z_pos_str}, {"x1", xStartStr}, {"x2", xStopStr}
		}, xz_reflector_elements);
	}

	/// for yz plane ///
	XMLObject * yz_reflector_elements = new XMLObject("g", "", {{"id", "detector-yz"}, {"stroke-width", "0.3"}, {"stroke", strokeColor}});
	for (int i = 0; i <= NyCubes; ++i) {
		int cube_y_pos = yStart + i * 10;
		std::string cube_y_pos_str = std::to_string(cube_y_pos);
		[[maybe_unused]] XMLObject * line = new XMLObject("line", "", {
			{"class", "reflector"}, {"x1", cube_y_pos_str}, {"x2", cube_y_pos_str}, {"y1", zStartStr}, {"y2", zStopStr}
		}, yz_reflector_elements);
	}

	for (int i = 0; i <= NzCubes; ++i) {
		int cube_z_pos = zStart + i * 10;
		std::string cube_z_pos_str = std::to_string(cube_z_pos);
		[[maybe_unused]] XMLObject * line = new XMLObject("line", "", {
			{"class", "reflector"}, {"x1", yStartStr}, {"x2", yStopStr}, {"y1", cube_z_pos_str}, {"y2", cube_z_pos_str}
		}, yz_reflector_elements);
	}

	/// for xy plane ///
	XMLObject * xy_reflector_elements = new XMLObject("g", "", {{"id", "detector-xy"}, {"stroke-width", "0.3"}, {"stroke", strokeColor}});
	for (int i = 0; i <= NxCubes; ++i) {
		int cube_x_pos = xStart + i * 10;
		std::string cube_x_pos_str = std::to_string(cube_x_pos);
		[[maybe_unused]] XMLObject * line = new XMLObject("line", "", {
			{"class", "reflector"}, {"x1", cube_x_pos_str}, {"x2", cube_x_pos_str}, {"y1", yStartStr}, {"y2", yStopStr}
		}, xy_reflector_elements);
	}

	for (int i = 0; i <= NyCubes; ++i) {
		int cube_y_pos = yStart + i * 10;
		std::string cube_y_pos_str = std::to_string(cube_y_pos);
		[[maybe_unused]] XMLObject * line = new XMLObject("line", "", {
			{"class", "reflector"}, {"x1", xStartStr}, {"x2", xStopStr}, {"y1", cube_y_pos_str}, {"y2", cube_y_pos_str}
		}, xy_reflector_elements);
	}

	return {xz_reflector_elements, yz_reflector_elements, xy_reflector_elements};
}


XMLObject * createMagneticFieldPointer(std::string plane, int x1, int y1, int x2, int y2, std::string color, XMLObject * parent)
{
	XMLObject * mfPrtGroup = new XMLObject("g", "", {{"id", "mf-ptr-" + plane}}, parent);
	if (plane == "xz" || plane == "xy"){
		XMLObject * mfPtr = new XMLObject("line", "", {
			{"x1", std::to_string(x1)},
			{"y1", std::to_string(y1)},
			{"x2", std::to_string(x2)},
			{"y2", std::to_string(y2)},
			{"stroke", color},
			{"stroke-width", "4"},
			{"marker-end", "url(#arrowhead)"},
			{"id", "mf-ptr-" + plane + "-arrow"},
			{"class", "mf-ptr-arrow"}
		}, mfPrtGroup);
		XMLObject * mfPtrLabel = new XMLObject("text", "B", {
			{"x", std::to_string(x2 + 60)}, {"y", std::to_string(y2 + 20)}, {"id", "mf-ptr-" + plane + "-label"}
		}, mfPrtGroup);
		mfPtrLabel->addAttribute("style", "font: bold 50px sans-serif; fill: #FFF");
	} else if (plane == "yz") {
		XMLObject * circle_outer = new XMLObject("circle", "", {
			{"r", "25"}, {"cx", std::to_string(x1)}, {"cy", std::to_string(y1)}, {"stroke-width", "5"}, {"stroke", color}
		}, mfPrtGroup);
		XMLObject * circle_inner = new XMLObject("circle", "", {
			{"r", "12"}, {"cx", std::to_string(x1)}, {"cy", std::to_string(y1)}, {"fill", color}
		}, mfPrtGroup);
		XMLObject * mfPtrLabel = new XMLObject("text", "B", {
			{"x", std::to_string(x2 + 40)}, {"y", std::to_string(y2 + 20)}, {"id", "mf-ptr-" + plane + "-label"}
		}, mfPrtGroup);
		mfPtrLabel->addAttribute("style", "font: bold 50px sans-serif; fill: #FFF");
	} else return mfPrtGroup;

	// XMLObject * mfPtrLabel = new XMLObject("text", "B", {
	// 	{"x", x2 + 10}, {"y", y2 + 10}, {"id", "mf-ptr-" + plane + "-label"}
	// }, {{"font", "bold 50px sans-serif"}, {"fill", "#FFF"}}, mfPrtGroup);

	// XMLObject * mfPtrLabel = new XMLObject("text", "B", {
	// 	{"x", std::to_string(x2 + 60)}, {"y", std::to_string(y2 + 20)}, {"id", "mf-ptr-" + plane + "-label"}
	// }, mfPrtGroup);
	// mfPtrLabel->addAttribute("style", "font: bold 50px sans-serif; fill: #FFF");

	return mfPrtGroup;
}


#endif