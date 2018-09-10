<!DOCTYPE html>
<html>
<head>
<?php
	include("common.php");
	htmlheads();
?>
</head>
<body>

<div class="container">

	<?php navbar(); ?>

	<!-- Jumbotron -->
	<!--<div class="jumbotron">
	<h1>CoDroid</h1>
	<p class="lead">An Open Framework for Measuring Android Application Code Coverage</p>
	</div>-->

	<div>
		<h2>Motivation</h2>

		<p>Although dynamic analysis techniques have been widely adopted, the capability of such tools is unclear, and a universal approach for measuring the capability of these tools remains unestablished. One major concern with dynamic analysis tools is that <i>they may not thoroughly inspect the application under testing</i>. Therefore, a fundamental metric for evaluating the capability of a dynamic analysis tool is measuring the code coverage for the tool. The code coverage is the ratio of codes that have been executed in a dynamic analysis environment. Code coverage is a common metric used in software testing and evaluation researches. Code coverage can be measured in diverse granularities including instruction-level, line-level, block-level, method-level, or class-level granularities. However, measuring code coverage is a non-trivial task, especially when a dynamic analysis tool is deployed only online. Researchers often measure code coverage by using dynamic instrumentation techniques, which introduce many constraints to the execution environments. In short, current code coverage measurement approaches can be applied only to limited environments and are unsuitable for performing large scale evaluations of dynamic analysis tools.</p>

		<p>We initiated the CoDroid proejct, an open framework that generates patched Android packages for measuring the code coverage of Android dynamic analysis tools. To the best of our knowledge, a publicly available tool that can be used to measure code coverage for mobile applications without source codes remains unestablished. CoDroid can generate packages for measuring the code coverage of online and offline tools. Using CoDroid, we conducted large-scale code coverage measurement for several popular dynamic analysis tools, including online tools (Anubis, Tracedroid, and NVISO ApkScan) and offline tools (Android emulator, DroidBox, and DroidScope). The presented approach can reveal the capability of a measured tool and provide comprehensive information for future improvement.</p>

		<h2>Design Philosophy</h2>

		<p>CoDroid provides an open framework for researchers to measure and determine the performance of dynamic analysis tools. The design objectives of CoDroid include the following:
		<ul>
		<li><em>No Dependency</em>. CoDroid measures code coverage at the binary level. No source code is required to use CoDroid. In addition, a patched application package can be launched alone without additional runtime components.</li>
		<li><em>Flexibility</em>. CoDroid measures both offline and online tools. Users can choose to store the measurement results in device storage spaces or send them to a specified data collection server.</li>
		<li><em>High Granularity</em>. CoDroid provides similar code coverage measurement granularities as those of Emma. Currently, granularities comprise class-level, method-level, block-level, and line-level granularities.</li>
		<li><em>Openness</em>. CoDroid will be publicly available at http://www.codroid.org/. Although using CoDroid in academic research is free of charge, researchers and developers should adhere to the license terms claimed in the binary and source packages.</li>
		</ul>
		</p>

		<h2>Overview</h2>

		<p>Figure 1 depicts a working flow overview of CoDroid. CoDroid has three major components, the patching module (the dot-dashed rectangle), the data collection module (the dotted rectangle), and the reporting module (the dashed rectangle). The patching module processes a given application package (apk) file and generates a patched apk file. The patching module can be configured to generate a package for measuring a local (offline) tool or remote (online) tool. In addition to generating packages, the patching module generates a metadata file, which stores required information to decode measurement results produced by the patched packages.</p>

		<p style="text-align: center">
		<img src="imgs/overview.png" width="60%"/><br/>
		Figure 1. Working flow overview of CoDroid.
		</p> 

		<p>Once a patched package is generated, it is launched with a local tool or submitted to a remote tool to measure the code coverage. A patched package for local measurements stores the measurement results in the internal storage of the running device as a single file. Users can retrieve the file and obtain the code coverage reports by using the reporting module. By contrast, a patched package for remote measurements sends fragments of measurement results to the data collection module. The reporting module then reads the fragments and the corresponding metadata file and generates the measurement reports. For the details of the design and implementation of CoDroid, please refer to our paper.</p>
	</div>

	<?php footer(); ?>

</div> <!-- /container -->

</body>
</html>
<!DOCTYPE html>
<html>
<head>
<?php
	include("common.php");
	htmlheads();
?>
</head>
<body>

<div class="container">

	<?php navbar(); ?>

	<!-- Jumbotron -->
	<!--<div class="jumbotron">
	<h1>CoDroid</h1>
	<p class="lead">An Open Framework for Measuring Android Application Code Coverage</p>
	</div>-->

	<div>
		<h2>Motivation</h2>

		<p>Although dynamic analysis techniques have been widely adopted, the capability of such tools is unclear, and a universal approach for measuring the capability of these tools remains unestablished. One major concern with dynamic analysis tools is that <i>they may not thoroughly inspect the application under testing</i>. Therefore, a fundamental metric for evaluating the capability of a dynamic analysis tool is measuring the code coverage for the tool. The code coverage is the ratio of codes that have been executed in a dynamic analysis environment. Code coverage is a common metric used in software testing and evaluation researches. Code coverage can be measured in diverse granularities including instruction-level, line-level, block-level, method-level, or class-level granularities. However, measuring code coverage is a non-trivial task, especially when a dynamic analysis tool is deployed only online. Researchers often measure code coverage by using dynamic instrumentation techniques, which introduce many constraints to the execution environments. In short, current code coverage measurement approaches can be applied only to limited environments and are unsuitable for performing large scale evaluations of dynamic analysis tools.</p>

		<p>We initiated the CoDroid proejct, an open framework that generates patched Android packages for measuring the code coverage of Android dynamic analysis tools. To the best of our knowledge, a publicly available tool that can be used to measure code coverage for mobile applications without source codes remains unestablished. CoDroid can generate packages for measuring the code coverage of online and offline tools. Using CoDroid, we conducted large-scale code coverage measurement for several popular dynamic analysis tools, including online tools (Anubis, Tracedroid, and NVISO ApkScan) and offline tools (Android emulator, DroidBox, and DroidScope). The presented approach can reveal the capability of a measured tool and provide comprehensive information for future improvement.</p>

		<h2>Design Philosophy</h2>

		<p>CoDroid provides an open framework for researchers to measure and determine the performance of dynamic analysis tools. The design objectives of CoDroid include the following:
		<ul>
		<li><em>No Dependency</em>. CoDroid measures code coverage at the binary level. No source code is required to use CoDroid. In addition, a patched application package can be launched alone without additional runtime components.</li>
		<li><em>Flexibility</em>. CoDroid measures both offline and online tools. Users can choose to store the measurement results in device storage spaces or send them to a specified data collection server.</li>
		<li><em>High Granularity</em>. CoDroid provides similar code coverage measurement granularities as those of Emma. Currently, granularities comprise class-level, method-level, block-level, and line-level granularities.</li>
		<li><em>Openness</em>. CoDroid will be publicly available at http://www.codroid.org/. Although using CoDroid in academic research is free of charge, researchers and developers should adhere to the license terms claimed in the binary and source packages.</li>
		</ul>
		</p>

		<h2>Overview</h2>

		<p>Figure 1 depicts a working flow overview of CoDroid. CoDroid has three major components, the patching module (the dot-dashed rectangle), the data collection module (the dotted rectangle), and the reporting module (the dashed rectangle). The patching module processes a given application package (apk) file and generates a patched apk file. The patching module can be configured to generate a package for measuring a local (offline) tool or remote (online) tool. In addition to generating packages, the patching module generates a metadata file, which stores required information to decode measurement results produced by the patched packages.</p>

		<p style="text-align: center">
		<img src="imgs/overview.png" width="60%"/><br/>
		Figure 1. Working flow overview of CoDroid.
		</p> 

		<p>Once a patched package is generated, it is launched with a local tool or submitted to a remote tool to measure the code coverage. A patched package for local measurements stores the measurement results in the internal storage of the running device as a single file. Users can retrieve the file and obtain the code coverage reports by using the reporting module. By contrast, a patched package for remote measurements sends fragments of measurement results to the data collection module. The reporting module then reads the fragments and the corresponding metadata file and generates the measurement reports. For the details of the design and implementation of CoDroid, please refer to our paper.</p>
	</div>

	<?php footer(); ?>

</div> <!-- /container -->

</body>
</html>
