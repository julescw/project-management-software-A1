/********* main.c ********
	
	Student Name 	= Jules Wong
	Student Number	= 101309829
*/

//Complete the includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "a1_functions.h"


int main()
{
    

    /** 1-  Display a welcome message **/
    printf("\nWelcome to ABC Company's Project Management Application!\nBest of luck with your project <3\n");

    /** 2- Get the project's name and number of milestones **/
    
    printf("Enter the project's name: ");
    char project_name[100];
    scanf("%s", &project_name);

    int num_milestones = my_get_input_usi("Enter the number of milestones (max 5): ");
    while(num_milestones > MAX_NUM_MILESTONES){ // Trap if too many milestones are entered //
        my_get_input_usi("Too many milestones. Try again\n");
    }

    /** 3- Initialize all milestones and activities.
     * You need an array of milestones. 
     * The capacity of the array must be the max number of milestones the program 
     * can handle as per the instructions **/

    milestone_t project_milestones[MAX_NUM_MILESTONES];
    int number_activities[MAX_NUM_MILESTONES];
    int used_activity_ids[MAX_NUM_MILESTONES * MAX_NUM_ACTIVITIES] = {-1};
    int used_activity_ids_pos = 0;
    char buffer[200];

    // looping through the milestones
    for(int i = 0; i < num_milestones; i++){ 
        printf("Initializing Milestone %d\n", i + 1);
        int temp_activity_num = my_get_input_usi("Enter the number of activities in this milestone: ");
        init_milestone(&project_milestones[i], temp_activity_num); // initialize milestone

        // activity IDs in single milestone have been verified as unique in init_milestone, now check across milestones
        while(true){
            _Bool dupe_found = false;
            for(int j = 0; j < temp_activity_num; j++){
                if (integer_array_contains(used_activity_ids, used_activity_ids_pos, project_milestones[i].activity_list[j].id)){
                    dupe_found = true;
                    break;
                }
            }
            if (dupe_found){ 
                printf("Some of the IDs for activities were duplicates or invalid.\n");
                for(int k = 0; k < temp_activity_num; k++){
                    sprintf(buffer, "Please enter the new ID for activity %s: \n", project_milestones[i].activity_list[k].name);
                    project_milestones[i].activity_list[k].id = my_get_input_usi(buffer);

                } 
                while(check_duplicate_activity_id(project_milestones->activity_list, temp_activity_num)){ 
                    printf("Some of the IDs for activities were duplicates or invalid.\n");
                    for(int m = 0; m < temp_activity_num; m++){
                        sprintf(buffer, "Please enter the new ID for activity %s: \n", project_milestones[i].activity_list[m].name);
                        project_milestones[i].activity_list[m].id = my_get_input_usi(buffer);
                    }
                }
            }
            if(! dupe_found){
                break;
            }
        }
        // add this milestone's set of unique IDs to the used IDs array to check other milestones
        for(int k = 0; k < temp_activity_num; k++){
            used_activity_ids[used_activity_ids_pos] = project_milestones[i].activity_list[k].id;
            used_activity_ids_pos++;
        }

        // putting the number of activities in the milestone into the number_activities array
        number_activities[i] = temp_activity_num;

    }

    // all milestones initialized, now check for unique milestone ids
    while(check_duplicate_milestone_id(project_milestones, num_milestones)){
        char my_buff[200];
        printf("Some of the IDs for milestones were duplicates or invalid.\n");
        for(int m = 0; m < num_milestones; m++){
            sprintf(my_buff, "Please enter the new ID for milestone %s: \n", project_milestones[m].name);
            int temp_id = my_get_input_usi(my_buff);
            project_milestones[m].id = temp_id;
        }
    }

    /** 4- Initialize the project **/
    project_t p1 = init_project(project_name, project_milestones, num_milestones, number_activities);

    /** 5- Print the main menu **/
    /** 6- Get user choice and handle it as per the instructions**/

    while(true){
        print_main_menu();
        unsigned short int choice = my_get_input_usi("\nEnter your choice: ");
        while((choice == 0)||(choice > 3)){
            choice = my_get_input_usi("Invalid choice. Enter your choice again: ");
        }
        if(choice == 1){
            print_sub_menu(project_milestones, num_milestones, number_activities);
            int update_activity_id = my_get_input_usi("Enter the activity ID you want to edit: \n");
            update_activity_by_id(project_milestones, update_activity_id, num_milestones, number_activities, &p1);
        }
        else if(choice == 2){
            print_project_stats(p1, project_milestones, num_milestones, number_activities);
        }
        else if (choice == 3){
            break;
        }
    }
    return EXIT_SUCCESS;
}