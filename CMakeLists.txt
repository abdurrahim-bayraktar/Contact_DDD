cmake_minimum_required(VERSION 4.0)
project(Contact_DDD)

set(CMAKE_CXX_STANDARD 20)

find_package(libpqxx REQUIRED)
find_package(Crow REQUIRED)

add_executable(Contact_DDD main.cpp
        infrastructure/connectionPooling.hpp
        Domain/callHistory/callHistory.hpp
        Domain/callHistory/callhistory-factory.hpp
        Domain/callHistory/callHistory-repository.hpp
        Domain/callHistory/callHistory-service.hpp
        Domain/contact/contact-service.hpp
        Domain/contact/contact.hpp
        Domain/contact/contact-factory.hpp
        Domain/contact/contact-repository.hpp
        application/application.hpp
        Domain/callHistory/value-objects/contact-info.hpp
        application/DTO/request-dto.hpp
        application/DTO/response-dto.hpp)

target_link_libraries(Contact_DDD PRIVATE libpqxx::pqxx)
target_link_libraries(Contact_DDD PRIVATE Crow::Crow)
