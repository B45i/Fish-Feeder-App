import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { IFeedSettings } from '../models/dto';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root',
})
export class FeederService {
  readonly bseURL = environment.bseURL;

  constructor(private http: HttpClient) {}

  getSettings(): Observable<IFeedSettings> {
    return this.http.get<IFeedSettings>(`${this.bseURL}/settings`);
  }

  saveSettings(settings: IFeedSettings): Observable<IFeedSettings> {
    return this.http.post<IFeedSettings>(`${this.bseURL}/settings`, settings);
  }
}
