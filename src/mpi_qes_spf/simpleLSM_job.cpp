#include "mpi_qes_spf/simpleLSM_job.h"
#include "string"

//#include "QESGui.h"

//#include "ViewTracer.h"
#include <boost/timer/timer.hpp>

// this is wallclock AND cpu time




bool simpleLSM_job::eval_population_fitness(population &pop)
{
    char pwd[FILENAME_MAX];
    getcwd(pwd, sizeof(pwd));
    log.debug("Current working directory", pwd);
    float cputime=0;
    float walltime=0;
    for (sample &s : pop)
    {
        prepare_work_dir_for_sample(s);
        boost::timer::cpu_timer timer;
        eval_sample_fitness(s);
        boost::timer::cpu_times elapsed = timer.elapsed();
        log.debug(" CPU TIME: ",  (elapsed.user + elapsed.system) / 1e9, " seconds", ", WALLCLOCK TIME: ", elapsed.wall / 1e9, " seconds");
        cputime+=((elapsed.user + elapsed.system) / 1e9);
        walltime+=(elapsed.wall / 1e9);

    }

    log.debug("total CPU TIME:", cputime, "total WALLCLOCK TIME:", walltime);        
    log.debug("avergae CPU TIME:", cputime/pop.size(), "avergae WALLCLOCK TIME:", walltime/pop.size());
    return true;
}

bool simpleLSM_job::eval_sample_fitness( sample &s)
{

    int max_gpus = 1;
    qes::QESContext context( max_gpus );

    // Now we create the Simple Land Surface Model.
    qes::RadiationTracer radModel;
    qes::SimpleLSM simpleLSM;
    qes::ViewTracer viewFactor;
    simpleLSM.setRadiationTracer( &radModel );
    context.joinModel( &radModel );
    context.joinModel( &simpleLSM );
    context.joinModel( &viewFactor );

    // Lets use another function to setup our scene.
    if ( !loadScene( &context ) )
    {
        return EXIT_FAILURE;
    }

    // Now that we've added all the models we want to use and created
    // our scene, it's time to initialize and compile the system. This
    // will return false if there is a problem.
    //context.getVariableTracker()->setBool("verbose_output", false);

    if ( !context.initialize() )
    {
        return EXIT_FAILURE;
    }

    //  runDiurnalCycle( &context );

    // Run a simulation.
	//may be get this from opt file
	qes::SunTracker *g_sunTracker = context.getSunTracker();

	int minute_interval = 1;
    g_sunTracker->updateTimeByMinutes( -minute_interval ); // just to handle the first iteration
    for ( int min = 0; min <= 2; min += minute_interval )
    {

    	log.debug("running simulation ", min+1, "th time");
        // This just adds n number of minutes to the current time.
        g_sunTracker->updateTimeByMinutes( minute_interval );

        // Run simulation
        if ( !context.runSimulation() )
        {
            return EXIT_FAILURE;
        }
    }
    // if ( !context.runSimulation() )
    // {
    //     return EXIT_FAILURE;
    // }

    // Now we will do something a bit more fun.
    // QUIC EnvSim has a default visualizer, and it's really easy to use.
    // To do this we must first create the GUI object:
    //  int screen_width = 1024;
    //  int screen_height = 768;
    //  qes::QESGui gui( screen_height, screen_width, &context );


    // Next, call display. This loads the render loop.
    // Pressing escape or closing the gui will return from this function
    // and the program can end.
    //  gui.display();


    // Exit the program. The QESContext takes care of all the cleanup!
    
    if(fitness->eval_fitness(s, &context))
        return EXIT_SUCCESS;
    else return EXIT_FAILURE;
}


bool simpleLSM_job::loadScene( qes::QESContext *context )
{

    qes::SceneTracker *g_sceneTracker = context->getSceneTracker();
    qes::SunTracker *g_sunTracker = context->getSunTracker();
    qes::VariableTracker *g_varTracker = context->getVariableTracker();

    std::stringstream project_file;
    //  project_file << QUIC_DATA_DIR << "/NakamuraOke1988/NakamuraOke1988.proj";
    project_file << QUIC_DATA_DIR << "/SLC_4m_extended/SLC_4m_extended.proj";
    //  project_file << QUIC_DATA_DIR << "/SimpleDomain2x2BuildingGrid/SimpleDomain2x2BuildingGrid.proj";
    //  project_file << "/scratch/quicTests/testGen_2x2/testGen_2x2.proj";
    //  project_file << QUIC_DATA_DIR << "/Mills/Mills.proj";

    g_sceneTracker->setUseAircells( false );
    bool scene_built = g_sceneTracker->initScene( workDir.outputDir+"/", "" );

    // Make sure the scene was built before doing anything else
    if ( !scene_built )
    {
        std::cout << "\n**Error building scene!  Exiting...\n" << std::endl;
        return false;;
    }

    // Before we run our simulation we should set it to a time where
    // the radiation model isn't full of errors. Let's do 2pm.
    int hour = 20; int minute = 0; int second = 0;
    g_sunTracker->setTimeUTC( hour, minute, second );
    g_sunTracker->setDate( 2014, 9, 15 );

    // We need to give our models input. This is done through the InputTracker
    // in the form of surface weather map (SWM) xml files. Each file contains
    // a list of observations for a given latitude, longitude, date, and time.
    // Each observation holds records such as air temperature and humidity.
    // So we need to load a file for SLC in October, 2011.
    //
    // First, get the tracker
    qes::InputTracker *g_inputTracker = context->getInputTracker();
    // Next, remove unneeded default data. This step is not necessary, but will
    // speed up observation retrieval during the simulation.
    g_inputTracker->clearAll();
    // Now load the surface weather map file stored in the resources directory.
    std::stringstream ss;
    ss << QES_ROOT_DIR << "/resources/SLCOct2011SWM.xml";
    g_inputTracker->loadSWMXML( ss.str() );

    // Store energy balance terms that are computed on the host
    //  g_varTracker->setBool( "store_terms", true );

    return true;

} // end load scene


void simpleLSM_job::runDiurnalCycle( qes::QESContext *context )
{

    // Minutes between simulations
    int minute_interval = 15;

    // This patch is just one in an urban canyon near the origin.
    // Just picking it for now...
    //  int patch_id = 107213;
    int patch_id = 0;

    // QES tools
    qes::SunTracker *g_sunTracker = context->getSunTracker();
    qes::BufferTracker *g_buffTracker = context->getBufferTracker();

    // Set starting date and time
    g_sunTracker->setDate( 2011, 10, 1 );
    g_sunTracker->setTimeLocal( 0, 0, 0 );

    // Output table of surface energy balance for designated patch id.
    std::map< int, std::vector<double> > seb_output; // minutes elapsed -> [ Rsn, Rl_dowm, Rl_up, Qg, Qh, Qe ]

    // The dirunal cycle loop
    g_sunTracker->updateTimeByMinutes( -minute_interval ); // just to handle the first iteration
    for ( int min = 0; min <= 1440; min += minute_interval )
    {

        // This just adds n number of minutes to the current time.
        g_sunTracker->updateTimeByMinutes( minute_interval );

        // Run simulation
        if ( !context->runSimulation() )
        {
            return;
        }

        // Get energy balance terms
        std::vector<double> Rs_terms;
        std::vector<double> Rl_down_terms;
        std::vector<double> Rl_up_terms;
        std::vector<double> Qg_terms;
        std::vector<double> Qh_terms;
        std::vector<double> Qe_terms;
        g_buffTracker->getHostBuffer<double>( "Rs_terms", &Rs_terms );
        g_buffTracker->getHostBuffer<double>( "Rl_down_terms", &Rl_down_terms );
        g_buffTracker->getHostBuffer<double>( "Rl_up_terms", &Rl_up_terms );
        g_buffTracker->getHostBuffer<double>( "Qg_terms", &Qg_terms );
        g_buffTracker->getHostBuffer<double>( "Qh_terms", &Qh_terms );
        g_buffTracker->getHostBuffer<double>( "Qe_terms", &Qe_terms );

        // Store energy balance terms
        std::vector<double> output_vec;
        //  output_vec.push_back( Rs_terms[ patch_id ] );
        //  output_vec.push_back( Rl_down_terms[ patch_id ] );
        //  output_vec.push_back( Rl_up_terms[ patch_id ] );
        double Rn = Rs_terms[ patch_id ] + Rl_down_terms[ patch_id ] - Rl_up_terms[ patch_id ];
        output_vec.push_back( Rn );
        output_vec.push_back( Qg_terms[ patch_id ] );
        output_vec.push_back( Qh_terms[ patch_id ] );
        output_vec.push_back( Qe_terms[ patch_id ] );
        seb_output.insert( std::pair< int, std::vector<double> >( min, output_vec ) );

    } // end diurnal cycle loop

    // Output energy balance terms to a file
    std::ofstream filestream;
    std::stringstream filename;
    filename << QES_OUTPUT_DIR << "/seb_output.txt";
    filestream.open( filename.str().c_str() );
    //  filestream << "minutes elapsed, Rsn, Rl_dowm, Rl_up, Qg, Qh, Qe" << std::endl; // header
    filestream << "minutes elapsed, Rn, Qg, Qh, Qe" << std::endl; // header

    // Loop over the output generated by the simulation loop
    std::map< int, std::vector<double> >::iterator it = seb_output.begin();
    for ( it; it != seb_output.end(); ++it )
    {

        // Get data
        int min_elapsed = it->first;
        std::vector<double> output_vec = it->second;

        // Output data to file
        filestream << min_elapsed;
        for ( int i = 0; i < output_vec.size(); ++i )
        {
            filestream << "\t" << output_vec[i];
        }
        filestream << std::endl;

    } // end loop seb output
    filestream.close();

} // end run diurnal cycle


