#include "Application.h"
#include "ApplicationFactory.h"
#include "ApplicationFactoryTestUtils.h"
#include "gtest/gtest.h"

/**
 * Due to the way GameNetworkingSockets works it isn't easy to run two applications at the same time.
 * I'll try and figure out a better way to test connection in the future.
 * For now, I will leave the code here before I realised that the tests don't work.
 */

// TEST(GTest, ServerSetsUp) {
//     int* frames = new int();
//     atomic_bool* server_ran = new atomic_bool();
//     ApplicationFactory* test_factory = CreateApplicationTestFactory(
//         [](chrono::milliseconds delta_time, ApplicationWindow& window) {},
//         [frames, server_ran](chrono::milliseconds delta_time, Application& application) {
//             if (*frames == 3 || *server_ran) application.Close_Application();
//             ++*frames;
//             if (application.Get_Network().Get_Network_State() == Network::Running) server_ran->store(true);
//         });
//     Create_Application(*test_factory, false);
//     ASSERT_TRUE(server_ran->load());
//     delete frames;
//     delete server_ran;
//     delete test_factory;
// }

// TEST(GTest, ClientConnectsToServer) {
//     int* server_frames = new int();
//     int* client_frames = new int();
//     atomic_bool* client_connected = new atomic_bool();
//     ApplicationFactory* server_test_factory = CreateApplicationTestFactory(
//         [](chrono::milliseconds delta_time, ApplicationWindow& window) {},
//         [server_frames, client_connected](chrono::milliseconds delta_time, Application& application) {
//             if (*server_frames == 100 || *client_connected) application.Close_Application();
//             ++*server_frames;
//         });
//     ApplicationFactory* client_test_factory = CreateApplicationTestFactory(
//         [](chrono::milliseconds delta_time, ApplicationWindow& window) {},
//         [client_frames, client_connected](chrono::milliseconds delta_time, Application& application) {
//             if (*client_frames == 0) application.Start_Client();
//             if (*client_frames == 10 || *client_connected) application.Close_Application();
//             ++*client_frames;
//             if (application.Get_Network().Get_Network_State() == Network::Connected) client_connected->store(true);
//         });
//
//     // thread server_thread(Create_Application, ref(*server_test_factory), false);
//     thread client_thread(Create_Application, ref(*client_test_factory), true);
//     client_thread.join();
//     // server_thread.join();
//     ASSERT_TRUE(client_connected->load());
//     delete client_connected;
//     delete server_frames;
//     delete client_frames;
//     delete client_test_factory;
//     delete server_test_factory;
// }
