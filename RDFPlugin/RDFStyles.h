#pragma once
#include "stdafx.h"
#include <fstream>
#include <nlohmann/json.hpp>

struct rdf_style {
    std::string name;
    int circleRadius;
    int circlePrecision;
    int circleThreshold;
    int lowAltitude;
    int highAltitude;
    int lowPrecision;
    int highPrecision;
    std::string rdfRGB;
    std::string rdfConcurRGB;
    bool drawController;
};

class StyleManager {
private:
    std::map<std::string, rdf_style> styles;
    std::string configPath;
    std::string defaultStyle = "LANGEN";  // Add this line to define default style

public:
    StyleManager(const std::filesystem::path& dllPath) {
        configPath = (dllPath.parent_path() / "RDFStyles.json").string();
        LoadStyles();
    }

    void LoadStyles() {
        styles.clear();
        try {
            std::ifstream f(configPath);
            if (!f.is_open()) {
                CreateDefaultConfig();
                f.open(configPath);
            }
            nlohmann::json j = nlohmann::json::parse(f);

            // Load default style name if it exists
            if (j.contains("default")) {
                defaultStyle = j["default"].get<std::string>();
            }

            // Load styles
            for (const auto& [key, value] : j.items()) {
                if (key == "default") continue;  // Skip the default entry

                rdf_style style;
                style.name = value["name"];
                style.circleRadius = value["circleRadius"];
                style.circlePrecision = value["circlePrecision"];
                style.circleThreshold = value["circleThreshold"];
                style.lowAltitude = value["lowAltitude"];
                style.highAltitude = value["highAltitude"];
                style.lowPrecision = value["lowPrecision"];
                style.highPrecision = value["highPrecision"];
                style.rdfRGB = value["rdfRGB"];
                style.rdfConcurRGB = value["rdfConcurRGB"];
                style.drawController = value["drawController"];

                styles[key] = style;
            }
        }
        catch (std::exception& e) {
            PLOGE << "Error loading styles: " << e.what();
        }
    }

    void CreateDefaultConfig() {
        nlohmann::json j;

        // Add default style setting
        j["default"] = "RING";

        j["LANGEN"] = {
            {"name", "Langen"},
            {"circleRadius", 20},
            {"circlePrecision", 0},
            {"circleThreshold", 999999},
            {"lowAltitude", 999999}, // disable the direction finder initially
            {"highAltitude", 0},
            {"lowPrecision", 10},
            {"highPrecision", 20},
            {"rdfRGB", "114:150:102"},
            {"rdfConcurRGB", "114:150:102"},
            {"drawController", false}
        };

        j["RING"] = {
            {"name", "Ring"},
            {"circleRadius", 20},
            {"circlePrecision", 0},
            {"circleThreshold", -1},
            {"lowAltitude", 999999}, // disable the direction finder initially
            {"highAltitude", 0},
            {"lowPrecision", 0},
            {"highPrecision", 0},
            {"rdfRGB", "114:150:102"},
            {"rdfConcurRGB", "114:150:102"},
            {"drawController", false}
        };

        std::ofstream f(configPath);
        f << j.dump(4);
    }

    const std::map<std::string, rdf_style>& GetStyles() const {
        return styles;
    }

    bool HasStyle(const std::string& name) const {
        return styles.find(name) != styles.end();
    }

    const rdf_style* GetStyle(const std::string& name) const {
        auto it = styles.find(name);
        return it != styles.end() ? &it->second : nullptr;
    }

    const rdf_style* GetDefaultStyle() const {
        return GetStyle(defaultStyle);
    }
};