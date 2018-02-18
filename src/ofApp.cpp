#include "ofApp.h"


 double img_dpi = 4800;
 double um2dot = img_dpi / 25.4 / 1000.0;
 double mm2dot = img_dpi / 25.4;
 double dot2um = 1.0 / um2dot;
 double dot2mm = 1.0 / mm2dot;

double img_width_mm = 5;
double img_height_mm = 5;

double gray_white = 0;			//White
double gray_black_ave = 228;	//Black
double gray_black_stdev = 0;

double dot_r_ave = 80 * um2dot;
double dot_r_stdev = 0.0*um2dot;

double gray_black_ave2 = 228;	//Black
double gray_black_stdev2 = 0;

double dot_r_ave2 = 80 * um2dot;
double dot_r_stdev2 = 0.0*um2dot;


double yy_sin_amp = 3.0*um2dot;
double yy_sin_pitch = 1.0;
double xx_sin_amp = 0.0*um2dot;
double xx_sin_pitch = 10.0;
double img_scale=1.0;
//画像の作成
int img_width = img_width_mm* mm2dot;
int img_height = img_height_mm * mm2dot;

double dot_y_stdev = 0.0*um2dot;
double dot_chile_stdev_um = 0;
double dot_chile_num = 0;



void image_prosess(Mat &img_proc, Mat &img_proc_scale) {
	 um2dot = img_dpi / 25.4 / 1000.0;
	 mm2dot = img_dpi / 25.4;
	 dot2um = 1.0 / um2dot;
	 dot2mm = 1.0 / mm2dot;




	img_proc = 255 - gray_white;
	double screen_grid_um = 25.4  * 1000.0 * 16.0 / 2400.0;
	double chile_um = 3.2* um2dot;

	cout << "screen_grid_num" << endl;

	cout << " img_width / screen_grid_um" << img_width / screen_grid_um << endl;
	cout << " img_height / screen_grid_um" << img_height / screen_grid_um << endl;
	cout << " img_width" << img_width << endl;
	cout << " img_height" << img_height << endl;
	cout << " screen_grid_um" << screen_grid_um << endl;
	cout << " um2dot" << um2dot << endl;

	std::vector<double> screen_x_um;
	std::vector<double> screen_y_um;
	std::vector<double> screen_y_um_noise;
	std::vector<double> screen_r_um;
	std::vector<double> screen_r_dense;

	std::vector<double> screen_r_um2;
	std::vector<double> screen_r_dense2;


	for (long j = 0; j < img_height / screen_grid_um / um2dot; j++) {
		for (long i = 0; i < img_width / screen_grid_um / um2dot; i++) {
			double xx = screen_grid_um*i;
			double yy = screen_grid_um*j;


			screen_x_um.push_back(xx);
			screen_y_um.push_back(yy);
			screen_x_um.push_back(xx + screen_grid_um / 2.0);
			screen_y_um.push_back(yy + screen_grid_um / 2.0);

			double dot_r1 = gaussian_random(dot_r_ave / 2.0, dot_r_stdev);
			double dot_r2 = gaussian_random(dot_r_ave / 2.0, dot_r_stdev);
			if (dot_r1 < 0) dot_r1 = 0.0;
			if (dot_r2 < 0) dot_r2 = 0.0;

			screen_r_um.push_back(dot_r1);
			screen_r_um.push_back(dot_r2);

			// dot_r1 = gaussian_random(dot_r_ave2 / 2.0, dot_r_stdev2);
			// dot_r2 = gaussian_random(dot_r_ave2 / 2.0, dot_r_stdev2);
			//if (dot_r1 < 0) dot_r1 = 0.0;
			//if (dot_r2 < 0) dot_r2 = 0.0;

			//screen_r_um2.push_back(dot_r1);
			//screen_r_um2.push_back(dot_r2);


			double dot_dense1 = gaussian_random(gray_black_ave, gray_black_stdev);
			double dot_dense2 = gaussian_random(gray_black_ave, gray_black_stdev);
			if (dot_dense1 < 0)dot_dense1 = 0;
			if (dot_dense1 > 255)dot_dense1 = 255;
			if (dot_dense2 < 0)dot_dense2 = 0;
			if (dot_dense2 > 255)dot_dense2 = 255;

			screen_r_dense.push_back(dot_dense1);
			screen_r_dense.push_back(dot_dense2);

			// dot_dense1 = gaussian_random(gray_black_ave2, gray_black_stdev2);
			// dot_dense2 = gaussian_random(gray_black_ave2, gray_black_stdev2);
			//if (dot_dense1 < 0)dot_dense1 = 0;
			//if (dot_dense1 > 255)dot_dense1 = 255;
			//if (dot_dense2 < 0)dot_dense2 = 0;
			//if (dot_dense2 > 255)dot_dense2 = 255;

			//screen_r_dense2.push_back(dot_dense1);
			//screen_r_dense2.push_back(dot_dense2);


		}
		double y_um_noise1 = gaussian_random(0 / 2.0, dot_y_stdev);
		double y_um_noise2 = gaussian_random(0 / 2.0, dot_y_stdev);
		screen_y_um_noise.push_back(y_um_noise1);
		screen_y_um_noise.push_back(y_um_noise2);

	}

	cout << "circle_generation" << endl;
	cout << "screen_x_um.size()" << screen_x_um.size() << endl;
	cout << "screen_y_um.size()" << screen_y_um.size() << endl;

	for (long i = 0; i < screen_x_um.size(); i++) {
		double xx_um = screen_x_um[i];
		double yy_um = screen_y_um[i] ;

		int nn = i / screen_y_um_noise.size();
		double yy_um_noise = screen_y_um_noise[nn];

		double xx_delt = xx_sin_amp*sin(2.0*M_PI*yy_um / 1000.0 / xx_sin_pitch);
		xx_um += xx_delt;

		double yy_delt = yy_sin_amp*sin(2.0*M_PI*yy_um / 1000.0 / yy_sin_pitch);
		yy_um += yy_delt + yy_um_noise;

		double dot_r = screen_r_um[i];
		int dot_dense = 255 - screen_r_dense[i];
		//double dot_r2 = screen_r_um[i]+screen_r_um2[i];
		//int dot_dense2 = 255 - screen_r_dense2[i];



		//cv::circle(img_proc, cv::Point(xx_um*um2dot, yy_um*um2dot), dot_r2, cv::Scalar(dot_dense2), -1, CV_AA);
		cv::circle(img_proc, cv::Point(xx_um*um2dot, yy_um*um2dot), dot_r, cv::Scalar(dot_dense), -1, CV_AA);

		for (int i = 0; i < dot_chile_num; i++) {
			double xx_um2 = xx_um + gaussian_random(0, dot_chile_stdev_um);
			double yy_um2 = yy_um + gaussian_random(0, dot_chile_stdev_um);

			cv::circle(img_proc, cv::Point(xx_um2*um2dot, yy_um2*um2dot), chile_um, cv::Scalar(dot_dense), -1, CV_AA);
		}
		
	}

	//Rect roi(0, 0, img_width/2 , img_width/2);
	//Rect roi(0, 0, 200, 200);
	//cv::resize(img_proc, img_proc_scale, img_proc_scale.size(), cv::INTER_AREA);
	//Mat img_proc_roi = img_proc(roi);
	img_proc_scale = img_proc;
}

//--------------------------------------------------------------
void ofApp::setup(){
	gui.setup("parametor");

	gui.add(screenSize.set("screenSize", ""));
	gui.add(doFullScreen.set("fullscreen (F)", false));
	gui.add(p11.setup("img_dpi", 4800, 1200, 10000));

	gui.add(bHide.set("GuiHide(h)", false));
	gui.add(p1.setup("dot_r_ave", 70, 0, 160));
	gui.add(p2.setup("dot_r_stdev", 0, 0, 20));
	gui.add(p3.setup("dot_density_ave", 50, 0, 255));
	gui.add(p4.setup("dot_density_stdev", 0, 0, 100));

	//gui.add(p12.setup("dot2_r_ave", 0, 0, 100));
	//gui.add(p13.setup("dot2_r_stdev", 0, 0, 20));
	//gui.add(p14.setup("dot2_density_ave", 255, 0, 255));
	//gui.add(p15.setup("dot2_density_stdev", 0, 0, 100));

	gui.add(p5.setup("yy_sin_amp",	0, 0, 100));
	gui.add(p6.setup("yy_sin_pitch", 1, 0.1, 10));
	gui.add(p7.setup("xx_sin_amp", 0, 0, 20));
	gui.add(p8.setup("xx_sin_pitch", 1, 0.1, 10));

	gui.add(p9.setup("image_size", 5, 1, 15));
	gui.add(p10.setup("dot_y_stdev", 0, 0, 50));
	
	gui.add(p16.setup("dot_chile_stdev_um", 40.0, 0.0, 120));
	gui.add(p17.setup("dot_chile_num", 0.0, 0.0, 600));

	doFullScreen.addListener(this, &ofApp::setFullScreen);
	gui.loadFromFile("settings.xml");

	img_dpi = p11;

	//画像の作成
	img_width = p9* mm2dot;
	img_height = p9 * mm2dot;
	img_proc = Mat::zeros(img_height, img_width, CV_8U);

	image_prosess(img_proc, img_proc_scale);
	
	image_out.allocate(img_proc_scale.cols, img_proc_scale.rows, OF_IMAGE_GRAYSCALE);
	image_out.setFromPixels(img_proc_scale.ptr(), img_proc_scale.cols, img_proc_scale.rows, OF_IMAGE_GRAYSCALE, false);
	event_flg = 0;



}

//--------------------------------------------------------------
void ofApp::update(){


	static double dot_r_ave_old;
	static double dot_r_stdev_old;
	static double gray_black_ave_old;
	static double gray_black_stdev_old;
	
	static double dot_r_ave2_old;
	static double dot_r_stdev2_old;
	static double gray_black_ave2_old;
	static double gray_black_stdev2_old;

	static double yy_sin_amp_old;
	static double yy_sin_pitch_old;
	static double xx_sin_amp_old;
	static double xx_sin_pitch_old;
	static double img_width_mm_old;
	static double dot_y_std_old;
	static double img_dpi_old;
	static double dot_chile_stdev_um_old;
	static double dot_chile_num_old;

	dot_r_ave = p1*um2dot;
	dot_r_stdev = p2*um2dot;
	gray_black_ave = 255-p3;
	gray_black_stdev = p4;

	dot_r_ave2 = p12*um2dot;
	dot_r_stdev2 = p13*um2dot;
	gray_black_ave2 = 255 - p14;
	gray_black_stdev2 = p15;

	dot_chile_stdev_um = p16;
	dot_chile_num = p17;

	yy_sin_amp = p5;
	yy_sin_pitch = p6;
	xx_sin_amp = p7;
	xx_sin_pitch = p8;
	img_width_mm = p9;
	dot_y_stdev = p10;
	img_dpi = p11;

	if (dot_r_ave != dot_r_ave_old)	event_flg = 1;
	if (dot_r_stdev != dot_r_stdev_old)	event_flg = 1;
	if (gray_black_ave != gray_black_ave_old)	event_flg = 1;
	if (gray_black_stdev != gray_black_stdev_old)	event_flg = 1;

	if (dot_r_ave2 != dot_r_ave2_old)	event_flg = 1;
	if (dot_r_stdev2 != dot_r_stdev2_old)	event_flg = 1;
	if (gray_black_ave2 != gray_black_ave2_old)	event_flg = 1;
	if (gray_black_stdev2 != gray_black_stdev2_old)	event_flg = 1;


	if (yy_sin_amp_old != yy_sin_amp)	event_flg = 1;
	if (yy_sin_pitch_old != yy_sin_pitch)	event_flg = 1;
	if (xx_sin_amp_old != xx_sin_amp)	event_flg = 1;
	if (xx_sin_pitch_old != xx_sin_pitch)	event_flg = 1;
	if (img_width_mm_old != img_width_mm)	event_flg = 1;
	if (dot_y_std_old != dot_y_stdev)	event_flg = 1;
	if (img_dpi_old != img_dpi)	event_flg = 1;
	if (dot_chile_stdev_um_old != dot_chile_stdev_um)	event_flg = 1;
	if (dot_chile_num_old != dot_chile_num)	event_flg = 1;


	if (event_flg) {
		

		event_flg = 0;
		
		img_width = img_width_mm* mm2dot;
		img_height = img_width_mm * mm2dot;
		img_proc = Mat::zeros(img_height, img_width, CV_8U);

		image_prosess(img_proc, img_proc_scale);
		image_out.resize(img_proc_scale.cols, img_proc_scale.rows);
		image_out.setFromPixels(img_proc_scale.ptr(), img_proc_scale.cols, img_proc_scale.rows, OF_IMAGE_GRAYSCALE, false);
	
		cout << "dot ave" <<255- (int)mean(img_proc_scale)[0] << endl;

	}


	dot_r_ave_old = dot_r_ave;
	dot_r_stdev_old = dot_r_stdev;
	gray_black_ave_old = gray_black_ave;
	gray_black_stdev_old = gray_black_stdev;

	dot_r_ave2_old = dot_r_ave2;
	dot_r_stdev2_old = dot_r_stdev2;
	gray_black_ave2_old = gray_black_ave2;
	gray_black_stdev2_old = gray_black_stdev2;

	yy_sin_amp_old = yy_sin_amp;
	yy_sin_pitch_old = yy_sin_pitch;
	xx_sin_amp_old = xx_sin_amp;
	xx_sin_pitch_old = xx_sin_pitch;
	img_width_mm_old = img_width_mm;
	dot_y_std_old = dot_y_stdev;
	img_dpi_old = img_dpi;

	dot_chile_stdev_um_old = dot_chile_stdev_um;
		dot_chile_num_old = dot_chile_num;


}

//--------------------------------------------------------------
void ofApp::draw(){
	image_out.draw(0, 0,ofGetHeight(), ofGetHeight());

	if (!bHide) {
		gui.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//event_flg = 1;


	if (key == 'h') {
		bHide = !bHide;
	}
	else if (key == 's') {
		gui.saveToFile("settings.xml");
	}
	else if (key == 'l') {
		gui.loadFromFile("settings.xml");
	}


	else if (key == 'f') {
		doFullScreen.set(!doFullScreen.get());
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



void img_info(Mat &img) {

	// 行数
	std::cout << "rows: " << img.rows << std::endl;
	// 列数
	std::cout << "cols: " << img.cols << std::endl;


	// 次元数（画像なので縦・横の2次元）
	std::cout << "dims: " << img.dims << std::endl;
	// サイズ（2次元の場合）
	std::cout << "size[]: " << img.size().width << "," << img.size().height << std::endl;
	// ビット深度ID
	std::cout << "depth (ID): " << img.depth() << "(=" << CV_8U << ")" << std::endl;
	// チャンネル数
	std::cout << "channels: " << img.channels() << std::endl;
	// （複数チャンネルから成る）1要素のサイズ [バイト単位]
	std::cout << "elemSize: " << img.elemSize() << "[byte]" << std::endl;
	// 1要素内の1チャンネル分のサイズ [バイト単位]
	std::cout << "elemSize1 (elemSize/channels): " << img.elemSize1() << "[byte]" << std::endl;
	// タイプ
	std::cout << "type (ID): " << img.type() << "(=" << CV_8UC3 << ")" << std::endl;
	// 要素の総数
	std::cout << "total: " << img.total() << std::endl;
	// ステップ数 [バイト単位]
	std::cout << "step: " << img.step << "[byte]" << std::endl;
	// 1ステップ内のチャンネル総数
	std::cout << "step1 (step/elemSize1): " << img.step1() << std::endl;
	// データは連続か？
	std::cout << "isContinuous: " << (img.isContinuous() ? "true" : "false") << std::endl;
	// 部分行列か？
	std::cout << "isSubmatrix: " << (img.isSubmatrix() ? "true" : "false") << std::endl;
	// データは空か？
	std::cout << "empty: " << (img.empty() ? "true" : "false") << std::endl;

}



// maxを最大とするint型の乱数を生成
int int_random(int max) {
	int i;
	return (int)(max*(rand() / (RAND_MAX + 1.0)));
}

// minを最小，maxを最大とするdouble型の乱数を生成
double uniform_random(double min, double max) {
	return min + (rand() / (double)RAND_MAX) * (max - min);
}

// 平均mean，標準偏差stdのガウス分布の乱数を生成
double gaussian_random(double mean, double std) {
	const double norm = 1.0 / (RAND_MAX + 1.0);
	double u = 1.0 - rand() * norm;
	double v = rand() * norm;
	double z = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);
	return mean + std * z;
}



