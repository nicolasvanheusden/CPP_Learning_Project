# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ?
-- Il faut appuyer sur la touche c.
Comment faire pour quitter le programme ?
-- Pour quitter le programme on peut appuyer sur x ou q.
A quoi sert la touche 'F' ?
-- La touche F sert à mettre en plein écran mais ne fonctionne pas sur Mac.

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
-- L'avion atterri puis se déplace vers une zone cargo et re-décolle.
Quelles informations s'affichent dans la console ?
EY3409 is now landing...
now servicing EY3409...
done servicing EY3409
EY3409 lift off

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?
-- 3 d'entre-eux atterissent et un reste en survol autour de 
l'aéroport en attente d'avoir une place disponible.

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.
-- À la racine du dossier src/ retrouve les classes suivantes :
    - AircraftType : représente le type d'un avion ainsi que ses caractéristiques.
    - Aircraft : est la classe de l'avion qui va interagir avec le display.
    - AirportType : est la classe qui contient les infos sur l'aéroport (ses pistes d'atterissage,etc..).
    - Airport : est la classe qui gère l'aéroport, (réserver un terminal, etc..).
    - Terminal : est la classe qui gère l'état d'un terminal.
    - TowerSimulation : est la classe qui est 'le plateau du jeu'; la classe qui initialise le jeu.
    - Tower : donne les instructions à un avion.
    - Waypoint : indique si un avion est dans les air ou sur terre.
    - Point3D : point avec 3 coordonées.
    - Point2D : point avec 2 coordonées.
    - Runway : représente une piste d'aéroport.

Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.
-- Pour la classe `Tower` les fonctions membres sont les suivantes : 
    ```
        /** Fonction qui donne les instructions à l'avion pour qu'il rejoigne l'aéroport.
        S'il est à porté de l'aéroport, on essaie de lui reserver une place,
        et on le fait tourner en rond si on a pas de place.
        S'il est dejà à l'aéroport, on lui trouve un moyen de re-décoller s'il a fini son service. */
        WaypointQueue get_instructions(Aircraft& aircraft);
    ```
    
    ```
        /** 
        Fonction qui assigne le debut d'un service pour un avion, une fois qu'il est arrivé à l'aéroport
        */
        void arrived_at_terminal(const Aircraft& aircraft);
    ```
-- Pour la classe `Aircraft` les fonctions membres sont les suivantes :
    ```
        /**
            Getter sur le numéro de vol de l'avion
        */
        const std::string& get_flight_num() const { return flight_number; }
    ```
    ```
        /**
            Fonction qui calcul la distance entre la position de l'avion et un point en 3D.
        */
        float distance_to(const Point3D& p) const { return pos.distance_to(p); }
    ```
    ```
        /** 
            Fonction qui affiche un avion à l'écran
        */ 
        void display() const override;
    ```
    ```
        /**
            Fonction qui déplace un avion
        */
        void move() override;
    ```
-- Pour la classe `Airport` les fonctions membres sont les suivantes :

    ```
        // Getter sur Tower.
        Tower& get_tower() { return tower; }
    ```

    ```
        // Fonction qui dessine l'aéroport.
        void display() const override { texture.draw(project_2D(pos), { 2.0f, 2.0f }); }
    ```

    ```
        // augmente le niveau de progression d'un terminal.
        void move() override
        {
            for (auto& t : terminals)
            {
                t.move();
            }
        }
    ```

-- Pour la classe `Terminal` les fonctions membres sont les suivantes :
    ```
        //Inidique si un terminal est actuellement utilisé par un avion.
        bool in_use() const { return current_aircraft != nullptr; }
    ```
    ```
        //Inidique si un terminal est en service
        bool is_servicing() const { return service_progress < SERVICE_CYCLES; }
    ```
    ```
        //Assigne a un avion ce terminal
        void assign_craft(const Aircraft& aircraft) { current_aircraft = &aircraft; }
    ```

    ```
        // Démarre le service d'un terminal
        void start_service(const Aircraft& aircraft)
        {
            assert(aircraft.distance_to(pos) < DISTANCE_THRESHOLD);
            std::cout << "now servicing " << aircraft.get_flight_num() << "...\n";
            service_progress = 0;
        }
    ```
    ```
        // Finis le service d'un terminal
        void finish_service()
        {
            if (!is_servicing())
            {
                std::cout << "done servicing " << current_aircraft->get_flight_num() << '\n';
                current_aircraft = nullptr;
            }
        }
    ```
    ```
        // Augmente la progression d'un service d'un terminal
        void move() override
        {
            if (in_use() && is_servicing())
            {
                ++service_progress;
            }
        }
    ```

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
-- La classe Tower avec la fonction get_instructions est impliquée dans la génération du chemin d'un avion.
-- La classe Waypoint représente des points de contrôles auquel l'avion doit passer.
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
`<std::dequeu>`
Expliquez les intérêts de ce choix.
std::deque (double-ended queue) is an indexed sequence container that allows fast insertion and deletion at both its beginning and its end. In addition, insertion and deletion at either end of a deque never invalidates pointers or references to the rest of the elements. 
## C- Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.
`Les vitesses maximales et accélération de chaque avion sont définies dans la class AircraftType`
`aircraft_types[2] = new AircraftType { .02f, .07f, .05f, MediaPath { "concorde_af.png" } };`

2) Identifiez quelle variable contrôle le framerate de la simulation.
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.
`constexpr unsigned int DEFAULT_TICKS_PER_SEC = 16u;`


3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
`constexpr unsigned int SERVICE_CYCLES = 40u;`

4) Lorsqu'un avion a décollé, il réattérit peu de temps après.
Faites en sorte qu'à la place, il soit retiré du programme.\
Indices :\
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?\
Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?
A quel endroit de la callstack pourriez-vous le faire à la place ?\
Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?
-- l'avion doit être supprimé quand il n'a plus de waypoint et qu'il a déjà atteri.
Il n'est pas sûr de procéder au retrait de l'avion dans cette fonction car la fonction move est appelée via un iterator sur le unordered_set move_queue. Quand on va supprimer un élément, il se peut qu'on saute un élément.
Il faudrait donc transmettre l'information à la méthode timer et retirer les avions en dehors de l'iterator.


5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.
Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour `DynamicObject` ?

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference ?
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.
