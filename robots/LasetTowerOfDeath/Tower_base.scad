//-- General defines
X = 0;
Y = 1;
Z = 2;

//-- User data
body_height = 25;
body_base_radius = 45;
body_top_radius = 39;


body_inner_height = 15;
body_inner_base_radius = 35;
body_inner_top_radius = 30;
r_dist_screws = sqrt( pow(15, 2) + pow(15, 2));


leg = [20, 15, 20];
leg_radius = 10;


module_base = [ 56, 56, 4.5];

drill_h = body_height - body_inner_height -0.5;

//-- Object description
difference()
{
	union()
	{
		//-- Turret body
		difference()
		{
			cylinder( r1 = body_base_radius, r2 = body_top_radius, h = body_height, $fn = 100 );
		
			translate( [0,0,-0.1] )
			cylinder( r1 = body_inner_base_radius, r2 = body_inner_top_radius, h = body_inner_height+0.1, $fn = 100);
		}
		
		//-- Turret legs
		for ( angle = [0:90:360])
			//-- Tranlate legs
			rotate( [0, 0, angle])
			translate( [body_base_radius + leg[Y]/3, 0, leg[Z] / 2.0 ])
			
			//-- Tranformations to place the extruded leg correctly
			rotate( [0, 0, 180])
			translate( -leg / 2)
			rotate( [ 90, 0, 0])
			translate( [0,0,-leg[Y] / 2]) 
			linear_extrude( height = leg[Y], center = true)
			polygon( points=[ [0,0], [leg[X], 0], [ leg[X], leg[Z]], [leg_radius, leg[Z]],
							 [0, leg[Z] - leg_radius]]);
			
	}
	
	union()
	{
		//-- Top drills
		for (angle = [0:90:360]) 
			translate( [r_dist_screws*cos(angle+45), r_dist_screws*sin(angle+45), body_height-drill_h])
			cylinder( r = 3/2, h = drill_h , $fn = 6); 
		
		//-- Module base
		translate( [0, 0, body_height-module_base[Z] / 2])
		minkowski(center = true)
		{
			cylinder( r = 5, h = 0.1, center = true, $fn = 100);
			cube( module_base, center = true );
		}
	}
}