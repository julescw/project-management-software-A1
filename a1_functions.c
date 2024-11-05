/********* a1_functions.c ********
	
	Student Name 	= Jules Wong
	Student Number	= 101309829
*/

#include <stdio.h>
#include <stdbool.h>
#include "a1_functions.h"


unsigned short int get_input_usi(void){
	
	int usi_input = 0;
	scanf("%d", &usi_input);
	while(usi_input < 0){ /*checks to make sure integer is positive*/
		printf("Not a valid integer, please try again: ");
		scanf("%d", &usi_input); /* continuously updates value if integer is invalid*/
	}
	return usi_input;
	}

unsigned short int my_get_input_usi(char *message){
	
	printf(message);
	return get_input_usi();
}

float get_input_f(void){
	
	float ft_input = 0;
	printf("Enter a float: ");
	scanf("%f", &ft_input); /* scanf uses pointers to assign value */
	while(ft_input < 0){
		printf("Not a valid float, please try again: ");
		scanf("%f", &ft_input);
	}
	return ft_input;
}

void init_activity(activity_t *activity_to_int){
	
	printf("Enter the activity's name: ");
	scanf("%s", activity_to_int->name);
	activity_to_int->id = my_get_input_usi("Enter the activity's ID: ");
	activity_to_int-> planned_duration = my_get_input_usi("Enter the planned duration in hours: ");
	activity_to_int-> planned_cost = activity_to_int->planned_duration * HOURLY_COST;
	activity_to_int-> completed = false; 
}

void init_milestone(milestone_t * milestone_to_int, unsigned short int num_activities){
	char buffer[200];
	printf("Enter the milestone's name: ");
	scanf("%s", milestone_to_int->name);
	milestone_to_int->id = my_get_input_usi("Enter the milestone's ID: ");

	for(int i = 0; i < num_activities; i++){
		init_activity(&(milestone_to_int->activity_list[i]));
	}
	
	while(check_duplicate_activity_id(milestone_to_int->activity_list, num_activities)){
		printf("Some of the IDs for activities were duplicates or NOT valid.\n");
		for(int j = 0; j < num_activities; j++){
			sprintf(buffer, "Please enter the new ID for activity %s: \n", milestone_to_int->activity_list[j].name);
			int temp_id = my_get_input_usi(buffer);
			milestone_to_int->activity_list[j].id = temp_id;
		}
    }
    milestone_to_int-> completed = false;
    milestone_to_int-> actual_cost = 0;
    milestone_to_int-> actual_duration = 0;
}

project_t init_project(char name[], milestone_t *milestone_list, int number_milestones, const int * number_activities){
	project_t *my_project = malloc(sizeof(project_t));
	strcpy(my_project->name, name);
	my_project->planned_cost = 0;
	int temp_planned_duration = 0;
	for(int i = 0; i < number_milestones; i++){
		for(int j = 0; j < number_activities[i]; j++){
			activity_t temp_activity;
			temp_activity = milestone_list[i].activity_list[j];
			my_project->planned_cost += temp_activity.planned_cost;
			temp_planned_duration += (temp_activity.planned_duration);
		}
	}
	my_project-> planned_duration = ceil(temp_planned_duration / 8.0);
	my_project-> actual_cost = 0;
	my_project-> actual_duration = 0;
    my_project-> completed = false;
	return *my_project;
}

void print_main_menu(void){
	printf("\n******** MAIN MENU ********\nPlease select one of the following options:\n1. Update Activity\n2. Print Stats\n3. Exit");
}

void print_milestone_stats(const milestone_t * list_milestones, int num_milestones, const int * number_activities){
	float mile_planned_cost = 0.0;
	int mile_planned_duration = 0;
	float planned_cost = 0;
	int planned_duration = 0; 
	for(int i = 0; i < num_milestones; i++){
		mile_planned_cost = 0.0;
		mile_planned_duration = 0;
		printf("Milestone Name: %s\n", list_milestones[i].name);
		printf("  Milestone ID: %d\n", (list_milestones + i)->id);
		printf("  Milestone Completion Status: %d\n", (list_milestones + i)->completed);
		if(list_milestones[i].completed){
			for(int k = 0; k < number_activities[i]; k++){
				planned_cost += (list_milestones + i)->activity_list[k].planned_cost;
				planned_duration += (list_milestones + i)->activity_list[k].planned_duration;
			}
			float budget = list_milestones[i].actual_cost - planned_cost;
			if(budget == 0){
			printf("  Budget: $0 over/under\n");
			}
			else if(budget < 0){
			printf("  Budget: Under by $ %.2f\n", fabs(budget));
			}
			else if(budget > 0){
			printf("  Budget: Over by $ %.2f\n", budget);
			}

			int schedule = ceil((list_milestones[i].actual_duration)/8.0) - ceil((planned_duration)/8.0);
			if(schedule == 0){
				printf("  Schedule: 0 days behind/ahead of schedule\n");
			}
			else if (schedule < 0){
				printf("  Schedule: Early by %d days\n", schedule);
			}
			else if (schedule > 0){
				printf("  Schedule: Late by %d days\n", schedule);
			}
		}
		else{
			for(int k = 0; k < number_activities[i]; k++){
				mile_planned_cost += (list_milestones + i)->activity_list[k].planned_cost;
				mile_planned_duration += (list_milestones + i)->activity_list[k].planned_duration;
			}
			printf("  Milestone Planned Cost: $ %.2f\n", mile_planned_cost);
			printf("  Milestone Planned Duration: %d hours\n", mile_planned_duration);
		}

		for(int j = 0; j < number_activities[i]; j++){
			printf("    Activity Name: %s\n", (list_milestones + i)->activity_list[j].name);
			printf("      Activity ID: %d\n", (list_milestones + i)->activity_list[j].id);
			printf("      Status: %u\n", (list_milestones + i)->activity_list[j].completed);
			if((list_milestones+i)->activity_list[j].completed){
				printf("      Actual Duration: %d hours\n", (list_milestones + i)->activity_list[j].actual_duration);
				printf("      Actual Cost: $ %.2f\n", (list_milestones + i)->activity_list[j].actual_cost);
			}
			else{
				printf("      Planned Duration: %d hours\n", (list_milestones + i)->activity_list[j].planned_duration);
				printf("      Planned Cost: $ %.2f\n", (list_milestones + i)->activity_list[j].planned_cost);
			}
		}
	}
}

void print_project_stats(project_t details, const milestone_t * list_milestones, int num_milestones, const int * number_activities){
    printf("Project Name: %s\n", details.name);
    printf("  Status: %d\n", details.completed);
	if(details.completed == true){
		float budget = details.actual_cost - details.planned_cost;
		if(budget == 0){
    	printf("  Budget: $0 over/under\n");
		}
		else if(budget < 0){
		printf("  Budget: Under by $ %.2f\n", fabs(budget));
		}
		else if(budget > 0){
		printf("  Budget: Over by $ %.2f\n", budget);
		}

		int schedule = details.actual_duration - details.planned_duration;
		if(schedule == 0){
			printf("  Schedule: 0 days behind/ahead of schedule\n");
		}
		else if (schedule < 0){
			printf("  Schedule: Early by %d days\n", schedule);
		}
		else if (schedule > 0){
			printf("  Schedule: Late by %d days\n", schedule);
		}

	}
	else if(details.completed == false){
		printf("  Planned Cost: $ %.2f\n", details.planned_cost);
    	printf("  Planned Duration: %d days\n", details.planned_duration);
	}
	print_milestone_stats(list_milestones, num_milestones, number_activities);
}

// need: function to check if milestone IDs are the same. function to check if activity IDs are the same

_Bool check_duplicate_activity_id(activity_t * list_to_check, int num_activities){
	_Bool is_dupe = false;
	if(num_activities == 1){
		is_dupe = false;
	}
	else if (num_activities == 2){
		if(list_to_check[0].id == list_to_check[1].id){
			is_dupe = true;
		}
	}
	else if(num_activities == 3){
		if((list_to_check[0].id == list_to_check[1].id)||(list_to_check[1].id == list_to_check[2].id)||(list_to_check[0].id == list_to_check[2].id)){
			is_dupe = true;
		}
	}
	return is_dupe;
}

_Bool check_duplicate_milestone_id(milestone_t * list_to_check, int num_milestones){
	_Bool is_dupe = false;
	int temp_id_list[MAX_NUM_MILESTONES] = {0};
	int temp_id_list_pos = 0;

	for(int i = 0; i < num_milestones; i++){
		if (integer_array_contains(temp_id_list, temp_id_list_pos, list_to_check[i].id)){
			is_dupe = true;
			break;
		}
		else{
			temp_id_list[temp_id_list_pos] = list_to_check[i].id;
			temp_id_list_pos ++ ;
		}
	}
	return is_dupe;
}

_Bool integer_array_contains(int *list_to_check, int length, int value){
	_Bool does_contain = false; 
	for(int i = 0; i < length; i++){
		if(list_to_check[i] == value){
			does_contain = true; 
			i = length;
		}
	}
	return does_contain;
}

void update_activity(activity_t * activity_to_update){
	int actual_duration = my_get_input_usi("Enter the actual duration of the activity: ");
	char completed;
	_Bool invalid = true;
	while(invalid){
		printf("Is the activity complete? (Y/N): ");
		scanf(" %c", &completed);
		if((completed == 'Y') || (completed == 'y') || (completed == 'N') || (completed == 'n')){
			invalid = false;
		}
	}
	if((completed == 'Y') || (completed == 'y')){
		activity_to_update->completed = true;
		activity_to_update->actual_cost = actual_duration * HOURLY_COST;
		activity_to_update->actual_duration = actual_duration;
	}
}


void update_milestone(milestone_t * milestone_to_update, int activities_in_milestone){
	_Bool fully_complete = true;
	int temp_num_activities = 0;
	int temp_actual_cost = 0;
	int temp_actual_duration = 0;
	// checking activities for completeness
	for(int i = 0; i < activities_in_milestone; i++){
		if(!milestone_to_update->activity_list[i].completed){
			fully_complete = false;
			break;
		}
	}
	// if all activities complete: set milestone to complete, compute actual cost and duration
	if(fully_complete){
		milestone_to_update->completed = true;
		for(int i = 0; i < activities_in_milestone; i++){
			temp_actual_cost += milestone_to_update->activity_list[i].actual_cost;
			temp_actual_duration += milestone_to_update->activity_list[i].actual_duration;
		}
		milestone_to_update->actual_cost = temp_actual_cost;
		milestone_to_update->actual_duration = temp_actual_duration;
	}
}


void update_project(const milestone_t * milestone_array, int num_milestones, const int * number_activities, project_t * my_project){
	_Bool fully_complete = true; 
	int temp_actual_cost = 0; 
	int temp_actual_duration = 0;
	// checking milestones for completeness
	for(int i = 0; i < num_milestones, i++;){
		if(!milestone_array[i].completed){
			fully_complete = false; 
			break;
		}
	}
	// if all milestones complete: set project to complete, compute actual cost and duration
	if(fully_complete){
		my_project->completed = true;
		for(int i = 0; i < num_milestones; i++){
			temp_actual_cost += milestone_array[i].actual_cost;
			temp_actual_duration += milestone_array[i].actual_duration;
		}
	my_project->actual_cost = temp_actual_cost;
	my_project->actual_duration = temp_actual_duration;
	}
	else{
		// no change
	}
}


void print_sub_menu(milestone_t * list_milestones, int num_milestones, const int * number_activities){
	for(int i = 0; i < num_milestones; i++){
		for(int j = 0; j < number_activities[i]; j++){
			if(list_milestones[i].activity_list[j].completed == 0){
				printf("Activity Name: %s\n", list_milestones[i].activity_list[j].name);
				printf("Activity ID: %d\n", list_milestones[i].activity_list[j].id);
				printf("Activity Status (1 is complete, 0 is incomplete): %d \n\n", list_milestones[i].activity_list[j].completed);
			}
		}
	}
}

void update_activity_by_id(milestone_t *list_milestones, int activity_id, int num_milestones, const int * number_activities, project_t *my_project){
	_Bool activity_found = false;
	int num_activities = 0;
	//loop through milestones 
	for(int i = 0; i < num_milestones; i++){
		// loop through activities
		for(int j = 0; j < number_activities[i]; j++){
			// attempt to find activity id in milestone 
			if(list_milestones[i].activity_list[j].id == activity_id){ 
				// found the activity id and calling update_activity
				update_activity(&(list_milestones[i].activity_list[j]));
				// calling update_milestone
				update_milestone(&list_milestones[i], number_activities[i]);
				// calling update_project
				update_project(list_milestones, num_milestones, number_activities, my_project);
			}	
		}	
	}
}