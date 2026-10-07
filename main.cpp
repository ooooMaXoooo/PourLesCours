#include <App.hpp>

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE, HINSTANCE, PSTR, int)
#else
int main()
#endif
{
    App app{};

    // Ajout des features.
    {
        app.reserve_features(2);

        app.add_feature("Convertisseur", std::make_unique<NoteConverter>());
        app.add_feature("Test", std::make_unique<NoteConverter>());
    }

    app.launch();
}