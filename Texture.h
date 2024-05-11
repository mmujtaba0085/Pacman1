#ifndef COURSEWORK_TEXTURES_H
#define COURSEWORK_TEXTURES_H


// Maze Textures
unsigned int maze_tex;
unsigned int pill_tex;
unsigned int bigPill_tex;

// Pacman Textures
unsigned int pac_0_tex;
unsigned int pac_1_tex;
unsigned int pac_2_tex;
unsigned int dead_0_tex;
unsigned int dead_1_tex;
unsigned int dead_2_tex;
unsigned int dead_3_tex;
unsigned int dead_4_tex;
unsigned int dead_5_tex;
unsigned int dead_6_tex;
unsigned int dead_7_tex;
unsigned int dead_8_tex;
unsigned int dead_9_tex;
unsigned int dead_10_tex;

// Ghost Textures
unsigned int ghost_r_0_tex;
unsigned int ghost_r_1_tex;
unsigned int ghost_p_0_tex;
unsigned int ghost_p_1_tex;
unsigned int ghost_b_0_tex;
unsigned int ghost_b_1_tex;
unsigned int ghost_y_0_tex;
unsigned int ghost_y_1_tex;
unsigned int ghost_scared_0_tex;
unsigned int ghost_scared_1_tex;
unsigned int ghost_scared_2_tex;
unsigned int ghost_scared_3_tex;

// Eye Textures
unsigned int eye_up_tex;
unsigned int eye_right_tex;
unsigned int eye_down_tex;
unsigned int eye_left_tex;

// UI Textures
unsigned int num_0_tex;
unsigned int num_1_tex;
unsigned int num_2_tex;
unsigned int num_3_tex;
unsigned int num_4_tex;
unsigned int num_5_tex;
unsigned int num_6_tex;
unsigned int num_7_tex;
unsigned int num_8_tex;
unsigned int num_9_tex;
unsigned int score_200_tex;
unsigned int score_400_tex;
unsigned int score_800_tex;
unsigned int score_1600_tex;
unsigned int ready_tex;
unsigned int gameover_tex;
unsigned int life_tex;
unsigned int score_tex;
unsigned int high_tex;
unsigned int score_100_tex;
unsigned int score_300_tex;

// Fruit Textures
unsigned int cherry_tex;
unsigned int strawberry_tex;



int png_load(const char* file_name, 
		     int* width, 
			 int* height, 
			 char** image_data_ptr)
{
    png_byte header[8];

    FILE* fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        fprintf(stderr, "erro: could not open PNG file %s\n", file_name);
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    if (width) { *width = temp_width; }
    if (height){ *height = temp_height; }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte* image_data;
    image_data = (png_byte*)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep* row_pointers = (png_bytep*)malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    //free(image_data);
	*image_data_ptr = (char*)image_data; // return data pointer

    free(row_pointers);
    fclose(fp);

	//fprintf(stderr, "\t texture image size is %d x %d\n", *width, *height);

	return 1;
}



unsigned int load_and_bind_texture(const char* filename)
{
    char* image_buffer = NULL; // the image data
	int width = 0;
	int height = 0;

	// read in the PNG image data into image_buffer
	if (png_load(filename, &width, &height, &image_buffer)==0)
    {
        fprintf(stderr, "Failed to read image texture from %s\n", filename);
        exit(1);
    }

	// request one texture handle
	unsigned int tex_handle = 0;
	glGenTextures(1, &tex_handle); 

	// create a new texture object and bind it to tex_handle
  	glBindTexture(GL_TEXTURE_2D, tex_handle);

	// so we can blend it 
  	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  	glTexImage2D(GL_TEXTURE_2D, 0, 
				GL_RGBA, width, height, 0,
   		 		GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image_buffer);

    glBindTexture(GL_TEXTURE_2D,0);

	free(image_buffer); // free the image buffer memory
	
	return tex_handle;
}






/**
 * Load and bind all textures at once to save performance as all only need to be load and bound once at the beginning during initialisation
 */
void loadAndBindTextures()
{
    // Enable blending so textures can be transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Bind map textures

    maze_tex =          load_and_bind_texture("sprites/maze/maze.png");
    pill_tex =          load_and_bind_texture("sprites/maze/p-0.png");
    bigPill_tex =       load_and_bind_texture("sprites/maze/p-1.png");

    // Bind Pacman textures
    pac_0_tex =         load_and_bind_texture("sprites/pacman/0.png");
    pac_1_tex =         load_and_bind_texture("sprites/pacman/1.png");
    pac_2_tex =         load_and_bind_texture("sprites/pacman/2.png");
    dead_0_tex =        load_and_bind_texture("sprites/pacman/d-0.png");
    dead_1_tex =        load_and_bind_texture("sprites/pacman/d-1.png");
    dead_2_tex =        load_and_bind_texture("sprites/pacman/d-2.png");
    dead_3_tex =        load_and_bind_texture("sprites/pacman/d-3.png");
    dead_4_tex =        load_and_bind_texture("sprites/pacman/d-4.png");
    dead_5_tex =        load_and_bind_texture("sprites/pacman/d-5.png");
    dead_6_tex =        load_and_bind_texture("sprites/pacman/d-6.png");
    dead_7_tex =        load_and_bind_texture("sprites/pacman/d-7.png");
    dead_8_tex =        load_and_bind_texture("sprites/pacman/d-8.png");
    dead_9_tex =        load_and_bind_texture("sprites/pacman/d-9.png");
    dead_10_tex =        load_and_bind_texture("sprites/pacman/d-10.png");

    // Bind ghost eye textures
    eye_up_tex =         load_and_bind_texture("sprites/eyes/up.png");
    eye_right_tex =      load_and_bind_texture("sprites/eyes/right.png");
    eye_down_tex =       load_and_bind_texture("sprites/eyes/down.png");
    eye_left_tex =       load_and_bind_texture("sprites/eyes/left.png");

    // Bind ghost textures
    ghost_r_0_tex =       load_and_bind_texture("sprites/ghosts/r-0.png");
    ghost_r_1_tex =       load_and_bind_texture("sprites/ghosts/r-1.png");
    ghost_p_0_tex =       load_and_bind_texture("sprites/ghosts/p-0.png");
    ghost_p_1_tex =       load_and_bind_texture("sprites/ghosts/p-1.png");
    ghost_b_0_tex =       load_and_bind_texture("sprites/ghosts/b-0.png");
    ghost_b_1_tex =       load_and_bind_texture("sprites/ghosts/b-1.png");
    ghost_y_0_tex =       load_and_bind_texture("sprites/ghosts/y-0.png");
    ghost_y_1_tex =       load_and_bind_texture("sprites/ghosts/y-1.png");
    ghost_scared_0_tex =  load_and_bind_texture("sprites/ghosts/s-0.png");
    ghost_scared_1_tex =  load_and_bind_texture("sprites/ghosts/s-1.png");
    ghost_scared_2_tex =  load_and_bind_texture("sprites/ghosts/s-2.png");
    ghost_scared_3_tex =  load_and_bind_texture("sprites/ghosts/s-3.png");

    // Bind UI textures
    num_0_tex =         load_and_bind_texture("sprites/ui/0.png");
    num_1_tex =         load_and_bind_texture("sprites/ui/1.png");
    num_2_tex =         load_and_bind_texture("sprites/ui/2.png");
    num_3_tex =         load_and_bind_texture("sprites/ui/3.png");
    num_4_tex =         load_and_bind_texture("sprites/ui/4.png");
    num_5_tex =         load_and_bind_texture("sprites/ui/5.png");
    num_6_tex =         load_and_bind_texture("sprites/ui/6.png");
    num_7_tex =         load_and_bind_texture("sprites/ui/7.png");
    num_8_tex =         load_and_bind_texture("sprites/ui/8.png");
    num_9_tex =         load_and_bind_texture("sprites/ui/9.png");
    score_200_tex =     load_and_bind_texture("sprites/ui/200.png");
    score_400_tex =     load_and_bind_texture("sprites/ui/400.png");
    score_800_tex =     load_and_bind_texture("sprites/ui/800.png");
    score_1600_tex =    load_and_bind_texture("sprites/ui/1600.png");
    ready_tex =         load_and_bind_texture("sprites/ui/ready.png");
    gameover_tex =      load_and_bind_texture("sprites/ui/gameover.png");
    life_tex =          load_and_bind_texture("sprites/ui/life.png");
    score_tex =         load_and_bind_texture("sprites/ui/score_tex.png");
    high_tex =          load_and_bind_texture("sprites/ui/high.png");
    score_100_tex =     load_and_bind_texture("sprites/ui/100.png");
    score_300_tex =     load_and_bind_texture("sprites/ui/300.png");

    // Fruit textures
    cherry_tex =        load_and_bind_texture("sprites/fruits/cherry.png");
    strawberry_tex =    load_and_bind_texture("sprites/fruits/strawberry.png");


}


void drawTex(unsigned int texture, int length, int height, float angle)
{
    // Begin new transformation matrix
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);   // Set background colour to white. This should never be seen but allows a visual debug to see if a texture hasn't
                                    // been loaded or bound correctly.

    int widthCentre = length/2;
    int verticalCentre = height/2;

    // Translate to center of sprite to rotate about its origin (for sprites such as pacman)
    glTranslatef((float)widthCentre,(float)verticalCentre,0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // Enable texturing and bind the specific sprite
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Specify texture coordinates
    glBegin(GL_QUADS);
        glTexCoord2f (0.0f, 0.0f);      // Lower left corner
        glVertex2i(-widthCentre, -verticalCentre);
        glTexCoord2f (1.0f, 0.0f);      // Lower right corner
        glVertex2i(widthCentre, -verticalCentre);
        glTexCoord2f (1.0f, 1.0f);      // Upper right corner
        glVertex2i(widthCentre, verticalCentre);
        glTexCoord2f (0.0f, 1.0f);      // Upper left corner
        glVertex2i(-widthCentre, verticalCentre);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Pop matrix to ignore above transformations on future objects
    glPopMatrix();
}







#endif 





