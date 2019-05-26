#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;
using std::cout;
using std::endl;

const int MIN_X = 0;
const int MAX_X = 100;
const int MIN_Y = 0;
const int MAX_Y = 100;

bool valuesInBounds(float x, float y){
    return (x>=MIN_X && x<=MAX_X) && (y>=MIN_Y && y<=MAX_Y);
}

void inputCoordinates(float& x, float& y, std::string string){
    char c;
    while(1){
        cout<<"Please enter x, y coordinates for "<< string <<" seperated by comma (0-100): ";
        std::cin>>x>>c>>y;
        if(valuesInBounds(x, y)){
            break;
        }
        cout<<"Error: Coordinate value(s) out of bound. Please enter values with the defined limits."<<endl;
    }
}

void printCoordinates(float x, float y, std::string string){
    cout<<string<<" coordinates = " << "[ " << x << ", "<< y << "]"<<endl;
}

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();
    std::vector<std::byte> contents(size);    
    
    is.seekg(0);
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);
}

int main(int argc, const char **argv)
{    
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;    
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
    // TODO: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
    // user input for these values using std::cin. Pass the user input to the
    // RoutePlanner object below.
    float start_x = 0, start_y=0, end_x =0, end_y =0;

    inputCoordinates(start_x, start_y, "start-point");
    inputCoordinates(end_x, end_y, "end-point");

    printCoordinates(start_x, start_y, "start-point");
    printCoordinates(end_x, end_y, "end-point");

    // Build Model.
    cout<<"Building model " <<endl;
    RouteModel model{osm_data};

    // Perform search and render results.
    cout<<"Planning route " <<endl;
    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    //RoutePlanner route_planner{model, 10, 10, 90, 90};

    cout<<"Initiating search " <<endl;
    route_planner.AStarSearch();
    cout<<"Rendering model " <<endl;

    Render render{model};

    std::cout<<"Distance = " << route_planner.GetDistance()<<std::endl;

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
