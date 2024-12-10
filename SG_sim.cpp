/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  SG_sim.cpp
 */

// This is our text-based SG simulator for debugging purposes
#include <iostream>
#include <random>
#include <cstdlib>
#include <string.h>
#include <math.h>

using namespace std;
const double pi = 3.14159265358979323846;

typedef struct
{   
    // Assuming all in z-basis and spin 1/2
    double theta, phi;

} state;

bool measure_z_spin(double num)
{
    if(num == 0)
    {
        return true; // Spin up
    }
    //else if(num == pi)
    return false; // Spin down
}

double square(double num)
{
    return (num*num);
}

state set_state(char input_direction, bool up)
{
    state output_state;

    if((input_direction == 'Z' || input_direction == 'z') && (up == true))
    {
        // +z  
        output_state.theta = 0;
        output_state.phi = 0;
    }
    else if((input_direction == 'Z' || input_direction == 'z') && (up == false))
    {
        // -z 
        output_state.theta = pi;
        output_state.phi = 0;
    }
    else if((input_direction == 'X' || input_direction == 'x') && (up == true))
    {
        // +x
        output_state.theta = pi/2;
        output_state.phi = 0;
    }
    else if((input_direction == 'X' || input_direction == 'x') && (up == false))
    {
        // -x 
        output_state.theta = pi/2;
        output_state.phi = pi;
    }
    else if((input_direction == 'Y' || input_direction == 'y') && (up == true))
    {
        // +y
        output_state.theta = pi/2;
        output_state.phi = pi/2;
    }
    else if((input_direction == 'Y' || input_direction == 'y') && (up == false))
    {
        // -y 
        output_state.theta = pi/2;
        output_state.phi = 3*pi/2;
    }
    else
    {
        cout << "Error" << endl;
    }

    return output_state;
}

// Stern-Gerlach in Z-direction (Prototype)
state SG_Z(state input_state)
{
    state output_state;

    // Probability Generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);
    double random_value = dist(gen);

    // From AS1
    // Spin up probability for z-direction
    double probability = square(cos(input_state.theta/2));

    if(random_value < probability)
    {
        output_state.theta = 0;
    }
    else // If not spin up, then spin down
    {
        output_state.theta = pi;
    }
    
    return output_state;
}

// General Stern-Gerlach Machine
state SG_Gen(state input_state, char direction)
{
    state output_state;
    double probability;

    // Probability Generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);
    double random_value = dist(gen);

    // Probability formulas from AS1
    if(direction == 'z' || direction == 'Z') // z-direction
    {
        // Spin up probability for z-direction
        probability = square(cos(input_state.theta/2));
    }
    else 
    if(direction == 'x' || direction == 'X') // x-direction 
    {
        // Spin up probability for x-direction
        probability = 0.5*(1 + sin(input_state.theta)*cos(input_state.phi));
    }
    else 
    if(direction == 'y' || direction == 'Y') // y-direction
    {
        // Spin up probability for y-direction
         probability = 0.5*(1 + sin(input_state.theta)*sin(input_state.phi));
    }

    if(random_value < probability)
    {
        output_state.theta = 0;
    }
    else // If not spin up, then spin down
    {
        output_state.theta = pi;
    }
    
    return output_state;
}

int main(void)
{
    int particle_amount, state_amount, i;
    int count_up_final = 0;
    int count_down_final = 0;
    double percentage_up, percentage_down, particles_left;
    state initial_state, output_state_final;
    string initial;
    char sg_direction[state_amount];
    int index;
    int block_spin_up_SG[2];

    cout << "---------------------------------------------------------------------------" << endl;

    cout << "How many SG machines? Enter between 1 to 3: ";
    cin >> state_amount;

    if(state_amount < 1 || state_amount > 3)
    {
        cout << "Invalid Input!" << endl;
        return 0;
    }
    else
    {
        for(i = 0; i < state_amount; i++)
        {
            index = i + 1;
            cout << "Enter the direction (Z, X, Y) of SG-" << index << ": ";
            cin >> sg_direction[i];

            if(sg_direction[i] != 'Z' && sg_direction[i] != 'z' && sg_direction[i] != 'X' && sg_direction[i] != 'x' && sg_direction[i] != 'Y' && sg_direction[i] != 'y')
            {
                cout << "Invalid Input!" << endl;
                return 0; 
            }
        }

        if(state_amount > 1)
        {    
            if(state_amount == 2 || state_amount == 3)
            {
                cout << "For output of 1st SG, input 0 to block spin down particles, or 1 to block spin up particles: ";
                cin >> block_spin_up_SG[0];

                if(block_spin_up_SG[0] != 0 && block_spin_up_SG[0] != 1)
                {
                    cout << "Invalid Input!" << endl;
                    return 0;
                }
            }

            if(state_amount == 3)
            {
                cout << "For output of 2nd SG, input 0 to block spin down particles, or 1 to block spin up particles: ";
                cin >> block_spin_up_SG[1];

                if(block_spin_up_SG[1] != 0 && block_spin_up_SG[1] != 1)
                {
                    cout << "Invalid Input!" << endl;
                    return 0;
                }
            }
        }

        cout << endl;
        cout << "Simulation Configuration: " << endl;

        if(state_amount != 1)
        {
            for(i = 0; i < state_amount; i++) 
            {
                cout << "SG-" << sg_direction[i];

                if(i < (state_amount - 1))
                {
                    if(block_spin_up_SG[i] == 0)
                    {
                        cout << " --SpinUp--> ";
                    }
                    else
                    {
                        cout << " -->SpinDown--> ";
                    }
                }
            }
            cout << " ---> Final Output" << endl;
        }
        else
        {
            cout << "SG-" << sg_direction[0] << " ---> Final Output" << endl;
        }
        
        cout << endl;
    }

    cout << "What is the initial state of particles?" << endl;
    cout << "+Z, -Z, +X, -X, +Y, -Y, Custom" << endl;
    cin >> initial;
    
    if(initial == "+Z" || initial == "+z")
    {
        initial_state.theta = 0;
        initial_state.phi = 0;
    }
    else if(initial == "-Z" || initial == "-z")
    {
        initial_state.theta = pi;
        initial_state.phi = 0;
    }
    else if(initial == "+X" || initial == "+x")
    {
        initial_state.theta = pi/2;
        initial_state.phi = 0;
    }
    else if(initial == "-X" || initial == "-x")
    {
        initial_state.theta = pi/2;
        initial_state.phi = pi;
    }
     else if(initial == "+Y" || initial == "+y")
    {
        initial_state.theta = pi/2;
        initial_state.phi = pi/2;
    }
    else if(initial == "-Y" || initial == "-y")
    {
        initial_state.theta = pi/2;
        initial_state.phi = 3*pi/2;
    }
    else if(initial == "custom" || initial == "Custom")
    {
        cout << "Enter theta value (in rad) between 0 to pi: "; 
        cin >> initial_state.theta;

        cout << "Enter phi value (in rad) between 0 to 2pi: "; 
        cin >> initial_state.phi;
        
        if((initial_state.theta < 0)||(initial_state.theta > pi)||(initial_state.phi < 0)||(initial_state.phi > 2*pi))
        {
            cout << "Invalid Input!" << endl;
            return 0;
        }
    } 
    else
    {
        cout << "Invalid Input!" << endl;
        return 0;
    }

    cout << "How many particles to start?" << endl;
    cin >> particle_amount;

    if(particle_amount > 0)
    {
        cout << "Chosen amount: " << particle_amount << endl;
    }
    else
    {
        cout << "Invalid Amount!" << endl;
        return 0;
    }
  
    // SG-Z for now, modify to choose SG direction
    cout << "Sending through SG configuration..." << endl;
    cout << endl;

    if(state_amount == 1)
    {
        for(i = 0; i < particle_amount; i++)
        {
            output_state_final = SG_Gen(initial_state, sg_direction[0]);
            if(measure_z_spin(output_state_final.theta) == true)
            {
                count_up_final++;
            }
            else
            {
                count_down_final++;
            }
        }
    }
    else
    if(state_amount == 2)
    {
        int count_up_1 = 0;
        int count_down_1 = 0;
        bool up_1;
        char sg_1_direction = sg_direction[0];
        char sg_2_direction = sg_direction[1]; 
        state output_state_1;

        // SG-1
        for(i = 0; i < particle_amount; i++)
        {
            output_state_1 = SG_Gen(initial_state, sg_direction[0]);
            if(measure_z_spin(output_state_1.theta) == true)
            {
                count_up_1++;
            }
            else
            {
                count_down_1++;
            }
        }
        
        // SG-2
        if(block_spin_up_SG[0] == 0) // Blocking spin down
        {
            up_1 = true;
            output_state_1 = set_state(sg_1_direction, up_1);

            for(i = 0; i < count_up_1; i++)
            {
                output_state_final = SG_Gen(output_state_1, sg_2_direction);
                if(measure_z_spin(output_state_final.theta) == true)
                {
                    count_up_final++;
                }
                else
                {
                    count_down_final++;
                }
            }
        }
        else // Blocking spin up
        {
            up_1 = false;
            output_state_1 = set_state(sg_1_direction, up_1);

            for(i = 0; i < count_down_1; i++)
            {
                output_state_final = SG_Gen(output_state_1, sg_2_direction);
                if(measure_z_spin(output_state_final.theta) == true)
                {
                    count_up_final++;
                }
                else
                {
                    count_down_final++;
                }
            }
        }
    }
    else
    if(state_amount == 3)
    {
        int count_up_1 = 0;
        int count_up_2 = 0;
        int count_down_1 = 0;
        int count_down_2 = 0;
        bool up_1, up_2;
        char sg_1_direction = sg_direction[0];
        char sg_2_direction = sg_direction[1];
        char sg_3_direction = sg_direction[2];
        state output_state_1;
        state output_state_2;

        // SG-1
        for(i = 0; i < particle_amount; i++)
        {
            output_state_1 = SG_Gen(initial_state, sg_1_direction);
            if(measure_z_spin(output_state_1.theta) == true)
            {
                count_up_1++;
            }
            else
            {
                count_down_1++;
            }
        }

        // SG-2
        if(block_spin_up_SG[0] == 0) // Blocking spin down
        {
            up_1 = true;
            output_state_1 = set_state(sg_1_direction, up_1);

            for(i = 0; i < count_up_1; i++)
            {
                output_state_2 = SG_Gen(output_state_1, sg_2_direction);
                if(measure_z_spin(output_state_2.theta) == true)
                {
                    count_up_2++;
                }
                else
                {
                    count_down_2++;
                }
            }
        }
        else // Blocking spin up
        {
            up_1 = false;
            output_state_1 = set_state(sg_1_direction, up_1);

            for(i = 0; i < count_down_1; i++)
            {
                output_state_2 = SG_Gen(output_state_1, sg_2_direction);
                if(measure_z_spin(output_state_2.theta) == true)
                {
                    count_up_2++;
                }
                else
                {
                    count_down_2++;
                }
            }
        }

        // SG-3
        if(block_spin_up_SG[1] == 0) // Blocking spin down
        {
            up_2 = true;
            output_state_2 = set_state(sg_2_direction, up_2);

            for(i = 0; i < count_up_2; i++)
            {
                output_state_final = SG_Gen(output_state_2, sg_3_direction);
                if(measure_z_spin(output_state_final.theta) == true)
                {
                    count_up_final++;
                }
                else
                {
                    count_down_final++;
                }
            }
        }
        else // Blocking spin up
        {
            up_2 = false;
            output_state_2 = set_state(sg_2_direction, up_2);

            for(i = 0; i < count_down_2; i++)
            {
                output_state_final = SG_Gen(output_state_2, sg_3_direction);
                if(measure_z_spin(output_state_final.theta) == true)
                {
                    count_up_final++;
                }
                else
                {
                    count_down_final++;
                }
            }
        }

    }    

    int particle_sum = count_up_final + count_down_final;
    percentage_up = 100*count_up_final/particle_sum;
    percentage_down = 100*count_down_final/particle_sum;
    particles_left = 100*particle_sum/particle_amount;
    
    int percentage_up_WRT_original = 100* count_up_final/particle_amount;
    int percentage_down_WRT_original = 100* count_down_final/particle_amount;

    cout << "Total particle throughput: " << particle_sum << "/" << particle_amount << " (" << particles_left << "%" << " of original amount)"<< endl;
    cout << "Particles measured with spin up: " << count_up_final << " (" << percentage_up << "%" << " of final output, " << percentage_up_WRT_original << "%" << " of original amount)" << endl; 
    cout << "Particles measured with spin down: " << count_down_final << " (" << percentage_down << "%" << " of final output, " << percentage_down_WRT_original << "%" << " of original amount)" << endl; 

    return 0;
}
