#include <App.hpp>

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE, HINSTANCE, PSTR, int)
#else
int main()
#endif
{
    App app{640, 480};

    // Ajout des features.
    {
        app.reserve_features(2);

        app.add_feature("Participation", std::make_unique<StudentRanking>());
        app.add_feature("Convertisseur", std::make_unique<NoteConverter>());
    }

    app.launch();
}