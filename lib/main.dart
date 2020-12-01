import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:async';
import 'package:flutter_tts/flutter_tts.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  FlutterTts flutterTts = FlutterTts();
  String data = "";
  void getdata() async {
    try {
      var url = 'http://192.168.43.116/';

      Timer.periodic(Duration(seconds: 1), (timer) async {
        var response = await http.get(url);
        if (response.statusCode == 200) {
          data = response.body;
          print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" + data);
          //if (int.parse(body) < 10) tts.speak("hello");
        } else {
          data = 'Request failed with status: ${response.statusCode}.';
          print('Request failed with status: ${response.statusCode}.');
        }
        setState(() {});
      });
    } catch (e) {
      print("Exception found:" + e.toString());
    }
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        home: Scaffold(
            appBar: AppBar(),
            body: Center(
              child: Column(
                children: <Widget>[
                  Text(data),
                  RaisedButton(onPressed: () {
                    getdata();
                  })
                ],
              ),
            )));
  }
}
