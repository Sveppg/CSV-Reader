#include "network.hpp"
#include "CSVReader.hpp"
#include "types.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <type_traits>

namespace bht {

Network::Network(const std::string& directoryPath) {
    // Laden der Daten aus den Dateien
    loadAgencies(directoryPath + "/agency.txt");
    loadCalendars(directoryPath + "/calendar.txt");
    loadLevels(directoryPath + "/levels.txt");
    loadPathways(directoryPath + "/pathways.txt"); 
    loadRoutes(directoryPath + "/routes.txt");
    loadShapes(directoryPath +"/shapes.txt");

}


//load different maps for different file types
void Network::loadAgencies(const std::string& filePath) {
    CSVReader agencyReader(filePath);
    while (agencyReader.hasNext()) {
        if (agencyReader.next()) {
            Agency agency;
            agency.id = agencyReader.getField("agency_id");
            agency.name = agencyReader.getField("agency_name");
            agency.url = agencyReader.getField("agency_url");
            agency.timezone = agencyReader.getField("agency_timezone");
            agency.language = agencyReader.getField("agency_lang");
            agency.phone = agencyReader.getField("agency_phone");
            agencies[agency.id] = agency;
        }
    }
}

void loadCalendarDates(const std::string filePath){
    CSVReader calendarDatesReader(filePath);
    while(calendarDatesReader.hasNext()){
        if(calendarDatesReader.next()){
            CalendarDate calendarDate;
            calendarDate.serviceId = calendarDatesReader.getField("service_ID");
            calendarDate.date = static_cast<GTFSDate>(std::(calendarDatesReader.getField("")));
        }
    }
}

void Network::loadCalendarDates(const std::string& filePath){
    CSVReader calendarDatesReader(filePath);
    while (calendarDatesReader.hasNext()) {
        if (calendarDatesReader.next()) {
            CalendarDate calendarDate;
            calendarDate.serviceId = calendarDatesReader.getField("service_id");

            std::string dateStr = calendarDatesReader.getField("date");
            calendarDate.date = {
                static_cast<unsigned char>(std::stoi(dateStr.substr(6, 2))),
                static_cast<unsigned char>(std::stoi(dateStr.substr(4, 2))),
                static_cast<unsigned short>(std::stoi(dateStr.substr(0, 4)))
            };

            calendarDate.exception = static_cast<CalendarDateException>(std::stoi(calendarDatesReader.getField("exception_type")));
            // Füge die CalendarDate-Datenstruktur zur entsprechenden Map hinzu, z.B.:
            calendarDates[calendarDate.serviceId] = calendarDate;
        }
    }
}


void Network::loadLevels(const std::string& filePath){
    CSVReader levelReader(filePath);
    while(levelReader.hasNext()){
        if(levelReader.next()){
            Level level;
            level.id = levelReader.getField("levels_id");
            level.index = std::stoi(levelReader.getField("index_level"));
            level.name = levelReader.getField("level_name");
            levels[level.id] = level;

        }
    }
}

void Network::loadPathways(const std::string& filePath){
    CSVReader pathwaysReader(filePath);
    while (pathwaysReader.hasNext()) {
        if (pathwaysReader.next()) {
            Pathway pathway;
            pathway.id = pathwaysReader.getField("pathway_id");
            pathway.fromStopId = pathwaysReader.getField("from_stop_id");
            pathway.toStopId = pathwaysReader.getField("to_stop_id");
            pathway.mode = static_cast<PathwayMode>(std::stoi(pathwaysReader.getField("pathway_mode")));
            pathway.isBidirectional = std::stoi(pathwaysReader.getField("is_bidirectional")) != 0;
            pathway.length = std::stof(pathwaysReader.getField("length"));
            pathway.traversalTime = static_cast<unsigned int>(std::stoi(pathwaysReader.getField("traversal_time")));
            pathway.stairCount = static_cast<unsigned int>(std::stoi(pathwaysReader.getField("stair_count")));
            pathway.maxSlope = std::stof(pathwaysReader.getField("max_slope"));
            pathway.minWidth = std::stof(pathwaysReader.getField("min_width"));
            pathway.signpostedAs = pathwaysReader.getField("signposted_as");
            pathways[pathway.id] = pathway;
        }
    }
}


void Network::loadRoutes(const std::string& filePath){
    CSVReader routesReader(filePath);
    while(routesReader.hasNext()){
        if(routesReader.next()){
            Route route;
            route.id = routesReader.getField("route_id");
            route.agencyId = routesReader.getField("agency_id");
            route.shortName = routesReader.getField("route_short_name");
            route.longName = routesReader.getField("route_long_name");
            route.description = routesReader.getField("route_desc");
            route.type = static_cast<RouteType>(std::stoi(routesReader.getField("route_type")));
            route.color = routesReader.getField("route_color");
            route.textColor = routesReader.getField("route_text_color");
            routes[route.id] = route;
        }       
    }
}

void Network::loadShapes(const std::string& filePath){
    CSVReader shapesReader(filePath);
    while(shapesReader.hasNext()){
        if(shapesReader.next()){
            Shape shape;
            shape.id = shapesReader.getField("shape_id");
            shape.latitide = std::stod(shapesReader.getField("shape_pt_lat"));
            shape.longitude = std::stod(shapesReader.getField("shape_pt_lon"));
            shape.sequence = std::stoi(shapesReader.getField("shape_pt_sequence"));         
            shapes[shape.id] = shape;
        }
    }
}

void Network::loadStopTimes(const std::string& filePath){
    CSVReader stopTimesReader(filePath);
    while (stopTimesReader.hasNext()) {
        if (stopTimesReader.next()) {
            StopTime stopTime;
            stopTime.tripId = stopTimesReader.getField("trip_id");

            std::string arrivalStr = stopTimesReader.getField("arrival_time");
            stopTime.arrivalTime = {
                static_cast<unsigned char>(std::stoi(arrivalStr.substr(0, 2))),
                static_cast<unsigned char>(std::stoi(arrivalStr.substr(3, 2))),
                static_cast<unsigned char>(std::stoi(arrivalStr.substr(6, 2)))
            };

            std::string departureStr = stopTimesReader.getField("departure_time");
            stopTime.departureTime = {
                static_cast<unsigned char>(std::stoi(departureStr.substr(0, 2))),
                static_cast<unsigned char>(std::stoi(departureStr.substr(3, 2))),
                static_cast<unsigned char>(std::stoi(departureStr.substr(6, 2)))
            };

            stopTime.stopId = stopTimesReader.getField("stop_id");
            stopTime.stopSequence = static_cast<unsigned int>(std::stoi(stopTimesReader.getField("stop_sequence")));
            stopTime.pickupType = static_cast<PickupType>(std::stoi(stopTimesReader.getField("pickup_type")));
            stopTime.dropOffType = static_cast<EDropOffType>(std::stoi(stopTimesReader.getField("drop_off_type")));
            stopTime.stopHeadsign = stopTimesReader.getField("stop_headsign");

            // Füge den StopTime-Datensatz der entsprechenden Map hinzu, z.B.:
            stopTimes[stopTime.tripId] = stopTime;
        }
    }
}

} 