/*****************************************************************//**
 * \file   Renderer.h
 * \brief  This file contains the implementaition of the Renderer class
 * 
 * \author Matěj
 * \date   May 2021
 *********************************************************************/

#pragma once

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include "Scene.h"
#include "../core/Camera.h"
#include "../core/Image.h"
#include "../shapes/Triangle.h"
#include "../materials/Material.h"


namespace glar
{
	/**
	 * This struct serves as a container for each thread. That means, each thread creates this container and fills it with its results 
	 */
	struct Block
	{
		int startIndex;
		int endIndex;
		std::vector<Color> colors;
	};
	
	class Renderer
	{
	public:
		/**
		 * \param camera		Camera to be used for rendering
		 * \param scenePath		Path to the scene file
		 * \param savePath		path to the file where the result should be saved
		 */
		Renderer(const Camera& camera, const std::string& scenePath, const std::string & savePath);

		/**
		 * Starts rendering process with given parameters.
		 * 
		 * \param samples			number of samples per pixel taht should be used
		 * \param bounces			how many times should the ray bounce in the scene
		 * \param threadsCount		how many threads should be used for the process
		 * \param flags				the rendering settings
		 */
		void render(int samples, int bounces, int threadsCount, RenderType flags = RenderType::INDIRECT);
	
	private:
		
		/**
		 * Function used by each thread to render its portion of an image
		 * 
		 * \param start			on which pixel the thread job starts
		 * \param end			on which pixel the thread job ends
		 * \param samples		number of samples per pixel
		 * \param bounces		number of bounces of each ray
		 * \param flags			rendering settings
		 */
		void threadJob(int start, int end, int samples, int bounces, RenderType flags);

		/**
		 * This function is a wrapper around the thread. It manages them, and at the end put the results of each thread together. 
		 * Its locked until all the hreads finish the work
		 * 
		 * \param samples			number of samples
		 * \param bounces			number of bounces
		 * \param threadsCount		number of threads defined by the caller
		 * \param flags				renderig settings
		 */
		void multiThreadRender(int samples, int bounces, int threadsCount, RenderType flags);
		

		/**
		 * power heuristic for the multiple importance sampling.
		 * 
		 * \param nf		number of samples of function f
		 * \param fPdf		PDF of the function f
		 * \param ng		number of samples of funtion g
		 * \param gPdf		PDF of the function g
		 * \return			the weight to be used in the monte carlo integration
		 */
		double powerHeuristic(int nf, double fPdf, int ng, double gPdf) const;

		/**
		 * this function serves as a wrapper aroun the indirect shading function, managing the function calls and settings
		 * 
		 * \param intersection	the point of which the color should be calculated
		 * \param bounces		number of bounces of each ray
		 * \param flags			rendering settings
		 * \return 
		 */
		Color indirectIllumination(const Intersection& intersection, int bounces, RenderType flags) const;


		/**
		 * this function serves as a wrapper around the direct shading function, managing the function calls and settings
		 *
		 * \param intersection	the point of which the color should be calculated
		 * \param flags			rendering settings
		 * \return
		 */
		Color directIllumination(const Intersection& intersection, RenderType flags) const;

		/**
		 * Counts only the direct illumination arriving to the point of interest
		 * 
		 * \param intersection		the point of interest
		 * \param light				the light to be used for the calculation
		 * \return					the counted color
		 */
		Color directShading(const Intersection& intersection, const std::shared_ptr<const Light>& light) const;

		/**
		 * Counts only the indirect illumination arriving to the point of interest
		 * 
		 * \param intersection		the point of interest
		 * \param bounces			number of bounces
		 * \param flags				rendering settings
		 * \return 
		 */
		Color indirectShading(const Intersection& intersection, int bounces, RenderType flags) const;

		Color m_background;							//background color
		std::string m_scenePath, m_savePath;;		//path to the scene and save files
		Image m_image;								//the image
		Scene m_scene;								// scene
		Camera m_camera;							// camera
		std::vector<Block> m_blocks;				// all the blocks of all threads
		std::mutex m_mutex;							//lock used for the block vector
		std::condition_variable m_cv;				
		std::atomic<int> m_completedThreads;		//number of finished threads
		std::vector<std::thread> m_threads;			//vector of all the threads
	};
}
