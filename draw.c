// reads rpn data and writes corresponding svg file
// default to in.rpn if no filename as argument
// default output to out.svg
// outputed SVG is scaled if '--scale' option is used (with a float)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //strtok strcat strcpy
#include <math.h>

#define TOKEN_SEPARATOR " "  // field delimiter
#define EOL "\n" // end of line 
#define COMMENT "#"
#define BUFFER_SIZE 256 // adjust to suit longest line

#define STROKE_WIDTH "0.1" // stroke width 
#define STROKE_COLOR "black" // stroke color
#define PSIZE 1 // point size, points are drawn as '+'

#define RAD2DEG 0.01745329
#define TRUE 1

#define RPN_FILENAME "in.rpn"
#define SVG_FILENAME "out.svg"

FILE * SVG_FILE;
FILE * RPN_FILE;
char out_filename[128];
float svg_scale = 1; // overall SVG scaling to fit 2100 x 2900 points A4 page


static inline float sind(float x) { return sin(x * RAD2DEG); }
static inline float cosd(float x) { return cos(x * RAD2DEG); }

int parse_loop(FILE *PARSED_FILE)
{
    char line_buffer [BUFFER_SIZE];

    while (TRUE) { // READ A LINE LOOP
        if (fgets(line_buffer, BUFFER_SIZE - 1, PARSED_FILE) == NULL) {
            break;
        }

        // break at end of file : where fgets returns 'NULL'
        //first token
        char *first = strtok(line_buffer, TOKEN_SEPARATOR);

        // EOL char will corrupt last token (filename in case 'inc')
        //
        // Option 1:
        // Before tokenizing temp, get rid of the newline as follows:
        //
        //  char *newline = strchr( temp, '\n' );
        //  if ( newline )
        //        *newline = 0;
        //
        // Option 2: (used below)
        // use 'end of line' as the last token separator'
        //
        if (strcmp(first, COMMENT) == 0) { // jump over comment
        } 
        //
        else if (strcmp(first, EOL) == 0) { // jump over empty line
        } 
        //
        else if ((strcmp(first, "a") == 0)) { // --------------------------- ARC
            float x = atof(strtok(NULL, TOKEN_SEPARATOR));
            float y = atof(strtok(NULL, TOKEN_SEPARATOR));
            float radius = atof(strtok(NULL, TOKEN_SEPARATOR));
            float start_angle = atof(strtok(NULL, TOKEN_SEPARATOR));
            float end_angle = atof(strtok(NULL, EOL));

            int largeArcFlag = ((end_angle - start_angle) > 180 );

            fprintf(SVG_FILE, "<path d= \"M %f %f A %f %f 0 %d 0 %f %f\"", 
                    x + (radius * cosd(end_angle)), // start x
                    y + (radius * sind(end_angle)), // start_y, 
                    radius, 
                    radius, 
                    largeArcFlag, 
                    x + (radius * cosd(start_angle)), // end x 
                    y + (radius * sind(start_angle)));  // end y 
            fprintf(SVG_FILE, " stroke = \"%s\" stroke-width = \"%s\" fill = \"none\"/>\n",
                    STROKE_COLOR, 
                    STROKE_WIDTH);
        } 
        //
        else if ((strcmp(first, "an") == 0)) { // ------------------------- ARCN
            float x = atof(strtok(NULL, TOKEN_SEPARATOR));
            float y = atof(strtok(NULL, TOKEN_SEPARATOR));
            float radius = atof(strtok(NULL, TOKEN_SEPARATOR));
            float start_angle = atof(strtok(NULL, TOKEN_SEPARATOR));
            float end_angle = atof(strtok(NULL, EOL));

            int largeArcFlag = ((end_angle - start_angle) > 180 );

            fprintf(SVG_FILE, "<path d= \"M %f %f A %f %f 0 %d 0 %f %f\"", 
                    x + (radius * cosd(start_angle)), // start x
                    y + (radius * sind(start_angle)), // start_y, 
                    radius, 
                    radius, 
                    largeArcFlag, 
                    x + (radius * cosd(end_angle)), // end x 
                    y + (radius * sind(end_angle)));  // end y 
                    fprintf(SVG_FILE, " stroke = \"%s\" stroke-width = \"%s\" fill = \"none\"/>\n",
                    STROKE_COLOR, 
                    STROKE_WIDTH); 
        } 
        //
        else if (strcmp(first, "B") == 0) { // -------------------- BEZIER CURVE
           
            fprintf(SVG_FILE, " <path d=\"M %f %f C %f %f %f %f %f %f\"",
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, EOL)));
            fprintf(SVG_FILE, " stroke=\"%s\" stroke-width=\"%s\" fill = \"none\"/>\n",
                    STROKE_COLOR,
                    STROKE_WIDTH);
        } 
        //
        else if (strcmp(first, "inc") == 0) { // ----------------------- INCLUDE
            // READ 4 TOKENS
            float x = atof(strtok(NULL, TOKEN_SEPARATOR)); // x pos
            float y = atof(strtok(NULL, TOKEN_SEPARATOR)); // y pos
            float s = atof(strtok(NULL, TOKEN_SEPARATOR)); // scale
            char * incfilename =    strtok(NULL, EOL);  // include filename
            FILE * INC_FILE = fopen(incfilename, "r");
            printf("Opening included file %s for reading\n", incfilename);

            if (INC_FILE == NULL) {
                fprintf(stderr, "! Error opening file %s\n", incfilename);
                return 1;
            }

            fprintf(SVG_FILE, "\n<g id=\"%s\" transform=\"translate(%f %f) scale (%f %f)\">\n\n",
                    incfilename, x, y, s, s);

            //
            // RECURSIVE CALL TO THIS LOOP
            //

            parse_loop(INC_FILE);
            fclose(INC_FILE);
            printf("Closed included file %s\n", incfilename);
            fprintf(SVG_FILE, "</g>\n\n");
        } 
        //
        else if (strcmp(first, "l") == 0) { // ---------------------------- LINE
            fprintf(SVG_FILE, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\"",
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, EOL)));
            fprintf(SVG_FILE, " stroke=\"%s\" stroke-width=\"%s\" />\n",
                    STROKE_COLOR,
                    STROKE_WIDTH);
        } 
        //
        else if (strcmp(first, "p") == 0) { // --------------------------- POINT
            float x = atof(strtok(NULL, TOKEN_SEPARATOR));
            float y = atof(strtok(NULL, EOL));
            fprintf(SVG_FILE, "%f %f moveto\n", x - PSIZE, y);
            fprintf(SVG_FILE, "%f %f lineto\n", x + PSIZE, y);
            fprintf(SVG_FILE, "%f %f moveto\n", x, y - PSIZE);
            fprintf(SVG_FILE, "%f %f lineto\n", x, y + PSIZE);
            fprintf(SVG_FILE, "stroke\n");            
        } else if (strcmp(first, "sc") == 0) { // ------------------------ SCALE
            float sc =  atof(strtok(NULL, EOL));
            fprintf(SVG_FILE, "%f %f scale\n", sc, sc);
        } else if (strcmp(first, "tr") == 0) { // -------------------- TRANSLATE
            fprintf(SVG_FILE, "%f %f translate\n",
                    atof(strtok(NULL, TOKEN_SEPARATOR)),
                    atof(strtok(NULL, EOL)));
        } else {
            fprintf(SVG_FILE, "%% ignored line\n");
        }
    } // end of loop

    return 0;
}


int open_input(int argc, char *argv[])
{
    // OPEN INPUT
    // open  file in  with a default filename (defined via  #define ) 
    // if no command-line argument is provided,
    const char *filename = (argc > 1) ? argv[1] : RPN_FILENAME;
    RPN_FILE= fopen(filename, "r");
    if (RPN_FILE == NULL) {
        perror("fopen failed");
        return 1;
    }    
    printf("Opened file : '%s' for reading\n", RPN_FILENAME);    
    return 0;
}


int open_output()
{
    // OPEN OUTPUT
    SVG_FILE = fopen(SVG_FILENAME, "w");

    if (SVG_FILE == NULL) {
    fprintf(stderr, "! Error opening file : '%s'\n", SVG_FILENAME);
        fclose(RPN_FILE);
        return 1;
    }
    fprintf(SVG_FILE, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"2100\" height=\"2970\">\n");
    fprintf(SVG_FILE, "<!-- \n");
    fprintf(SVG_FILE, "   the whole drawing <g> </g> is flipped over Y axis\n");
    fprintf(SVG_FILE, "   origin is set center bottom \n");
    fprintf(SVG_FILE, "   of the A4 sized page to match postscript output coordinates\n");
    fprintf(SVG_FILE, "-->\n");
    fprintf(SVG_FILE, "<g id=\"drawing\" transform=\"translate(1050 2900) scale (%.2f %.2f)\">\n", svg_scale, -svg_scale);
    fprintf(SVG_FILE, "\n");

    printf("Opened file : '%s' for writing, added SVG header\n", SVG_FILENAME);

    return 0;
}


int close_files(void)
{
    // rpn file
    fclose(RPN_FILE);
    printf("Closed file : '%s'\n", RPN_FILENAME);
    // svg file
    fprintf(SVG_FILE, "\n</g>\n");
    fprintf(SVG_FILE, "\n</svg>\n");
    fclose(SVG_FILE);
    printf("Closed file : '%s'\n\n", SVG_FILENAME);
    return 0;
}


int main(int argc, char * argv[])
{
    char * option_value = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--scale") == 0 && i + 1 < argc) {
            svg_scale = atof(argv[i + 1]);  // Converts "1.5" → 1.5f
            break;  // Or continue to allow multiple options
        }
    }
    open_input(argc, argv);
    open_output();
    parse_loop(RPN_FILE);
    close_files();
    return 0;
}
